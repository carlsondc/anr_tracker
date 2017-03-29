/**
 * Wiring:
 * ard    dev     pin   purpose       
 * 15     Trellis INT  
 * Vcc    Trellis 5v
 * GND    Trellis GND
 * 2      Trellis SDA
 * 3      Trellis SCL
 * 4      LED     4     CC2
 * 5      LED     5     D
 * 6      LED     6     F
 * 7      LED     7     G
 * 8      LED     8     B
 * 9      LED     9     CC1
 * 10     LED     10    A
 * 14     LED     3     E
 * 16     LED       1     C
 */


#include "Adafruit_Trellis.h"
//Digits uses a bitmap for the segment (A=MSB, DP = LSB).
unsigned char digits[] = {
  0b11111100, //0 A B C D E F
  0b01100000, //1   B C 
  0b11011010, //2 A B   D E   G
  0b11110010, //3 A B C D     G
  0b01100110, //4   B C     F G
  0b10110110, //5 A   C D   F G
  0b10111110, //6 A   C D E F G
  0b11100000, //7 A B C
  0b11111110, //8 A B C D E F G
  0b11110110, //9 A B C D   F G
  0b00000000, //off
};

//Arduino Pins
#define SEGMENT_A 10
#define SEGMENT_B 8
#define SEGMENT_C 16
#define SEGMENT_D 5
#define SEGMENT_E 14
#define SEGMENT_F 6
#define SEGMENT_G 7
#define SEGMENT_DP 

//Arduino Pins
#define DIGIT_1   9
#define DIGIT_2   4

int pinMapping[] {
  SEGMENT_A,
  SEGMENT_B,
  SEGMENT_C,
  SEGMENT_D,
  SEGMENT_E,
  SEGMENT_F,
  SEGMENT_G,
};

int digitMapping[] {
  DIGIT_1,
  DIGIT_2
};

#define TRELLIS_INT 15

#define CREDIT_UP 3
#define CREDIT_DOWN 7
#define CLICK_0 12
#define CLICK_1 13
#define CLICK_2 14
#define CLICK_3 15

//To display a digit, test MSB and set corresponding pin, shift left, repeat
Adafruit_Trellis trellis = Adafruit_Trellis();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("starting");
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(TRELLIS_INT, INPUT);
  digitalWrite(TRELLIS_INT, HIGH);
  for (int i = 0; i < sizeof(pinMapping) / sizeof(pinMapping[0]); i++){
    pinMode(pinMapping[i], OUTPUT);
  }
  trellis.begin(0x70);
}

void displayDigit(char toDisplay, int digitToShow) {
  digitalWrite(DIGIT_1, HIGH);
  digitalWrite(DIGIT_2, HIGH);
  unsigned char bitmap = digits[toDisplay];
  unsigned char mask = 0b10000000;
  for (int i=0; i < sizeof(bitmap) * 8; i++) {
    digitalWrite(pinMapping[i], mask & bitmap);    
    mask >>=1;
  }
  digitalWrite(digitMapping[digitToShow], LOW);
  trellis.setBrightness(10);
}


#define DECIMATION_FACTOR 2
#define NUM_DIGITS 2
int digit = 0;
#define DISPLAY_PERIOD_MS 10
#define INCREMENT_PERIOD_MS 250
#define TRELLIS_READ_PERIOD_MS 100
#define TRELLIS_DISPLAY_PERIOD_MS 200
#define COUNTER_SCALE 4

unsigned long lastDisplayChange = 0;
unsigned long lastIncrement = 0;
unsigned long lastTrellisUpdate = 0;
int lastKeymap = 0;

void loop() {
  static bool stillPressed = false;
  static bool lastPressed = false;
  static int counter = 5 * COUNTER_SCALE;
  if (digitalRead(TRELLIS_INT) == 0 || stillPressed){
    if ((millis() - lastTrellisUpdate) > TRELLIS_READ_PERIOD_MS){
      //This appears to affect the timing in some subtle way: without it, the credit display gets totally fucked up.
//      Serial.print(".");
      
      lastTrellisUpdate = millis();
      trellis.readSwitches();
      
      int keymap = 0;
      int ledMap = 0;
      for (int k=0; k< 16; k++){
        if (trellis.isKeyPressed(k)){
          keymap |= (1 << k);  
        }
        switch (k) {
          case CREDIT_UP:
            counter += (trellis.isKeyPressed(k))? 1 : 0;
            trellis.setLED(k, trellis.isKeyPressed(k)); 
            break;
          case CREDIT_DOWN:
            counter -= (trellis.isKeyPressed(k))? 1 : 0;
            trellis.setLED(k, trellis.isKeyPressed(k)); 
            break;
          case CLICK_0:
          case CLICK_1:
          case CLICK_2:
          case CLICK_3:
            break;
        }
      }
      if (trellis.isKeyPressed(CREDIT_UP) && trellis.isKeyPressed(CREDIT_DOWN)){
        counter = 5 * COUNTER_SCALE;
      } else {    
        if (trellis.isKeyPressed(CREDIT_UP)){
          counter++;
        } 
        if (trellis.isKeyPressed(CREDIT_DOWN)){
          counter--;
        }
      }
      
      if (keymap != lastKeymap) {
        stillPressed = (keymap != 0);
        for (int k = CLICK_0; k <= CLICK_3; k++){
          if ((keymap ^ lastKeymap) & (keymap & (1 << k))) {
            trellis.setLED(k, trellis.isLED(k)? 0 : 1);
          }
        }
        trellis.writeDisplay();
      } else {
        // nuffin 
      }
      if (counter < 0) {
        counter = 0;
      }
      lastKeymap = keymap;      
    }
  }

  if ((millis() - lastDisplayChange) >= DISPLAY_PERIOD_MS){
    lastDisplayChange = millis();
    digit++;
    switch (digit % NUM_DIGITS) {
      case 0:
        displayDigit(((counter/COUNTER_SCALE) / 10)%10, 0);
        break;
      case 1:
        displayDigit((counter/COUNTER_SCALE) %10, 1);
        break;
      default:
        break;
    }
  }
//  if (Serial.available()){
//    Serial.read();
//    Serial.println(post-pre);
//  }
  
}
