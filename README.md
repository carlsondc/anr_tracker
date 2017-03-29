Arduino project, SketchUp designs, and instructions for building a
Click/Credit Tracker for use with Android: Netrunner (copyright
Fantasy Flight Games)

Parts list
----------
- Silicon elastomer keypad: https://www.adafruit.com/products/1611
- Trellis driver PCB: https://www.adafruit.com/products/1616
- 3mm diffused LEDs of your choice 
  - https://www.adafruit.com/products/778
- Arduino Pro Micro
  - https://www.sparkfun.com/products/12640
  - should *probably* work with any Arduino, though
- 2-digit LED counter
  - http://www.digikey.com/product-detail/en/lite-on-inc/LTD-4708JR/160-1539-5-ND/408212 
- on/off switch https://www.sparkfun.com/products/102  
- 4xAA battery holder:
  http://www.digikey.com/product-detail/en/mpd-memory-protection-devices/BC4AAPC/BC4AAPC-ND/2329366 

Dependencies
------------
- Adafruit Trellis arduino library:
  https://github.com/adafruit/Adafruit_Trellis_Library
- Sparkfun Arduino board manager (for Pro Micro support)
  https://github.com/sparkfun/Arduino_Boards

Instructions
------------
- Print the 3 parts in tracker.skp (exported to various .stl's for
  convenience)
- Following the pinout described in tracker.ino, connect the Trellis
  PCB and LED display to the Arduino
  - recommend mounting the LED display on 0.1" perf board -- gives it
    a flat surface to sit on and keeps it more flush with everything.
- Connect arduino ground to battery ground, battery+ to middle pin of
  switch, and arduino RAW to one of the side pins of the switch
- cut off the unused buttons from the silicon keypad. Cover that area
  with electrical tape to prevent shorts with the bottom of the LED
  display.
- stuff all that shit back in the box. Tape anything that might short.
- go man go!

TODO:
-----

- The 4 corner legs on the top lid are a little too wide (when printed
  on a MakerBot). They should lose probably ~0.1 mm off each outside
  face.
- The decimal point on the LCD display is not used. I've currently
  wired it to A0, but it's not referenced from the code.
- The button detection for credit +/- is a bit touchy. Should go one
  full step on button-down, and on button-up we should drop any
  fractional counts (round down to currently-displayed val).
- Verify on boards other than arduino pro micro
- Add better latching mechanism-- either tabs to grip inside wall or
  receptacles for a threaded sleeve/screw
