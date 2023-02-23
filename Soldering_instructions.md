# Soldering instructions (for Loconet)

1. See which lights you have. By default 5V is applied with a 330 Ohm resistor (R20 and R21) in series.  
In case your lights are not bright enough, there are three options:  
-  1) short the 330 Ohm resistors (JP1 and JP2). 
-  2) increase the voltage to the light to the Loconet Voltage by cutting the jumper on RAW_5V0 with a sharp knife (measure with a multimeter if there if the pads are really seperate) and put a solder blob between the middle pad and the pad below the RAW text. 
-  3) combine 1) and 2).

2. First decide if you want to use the a DC/DC converter (e.g. XP Power VR05S05) or to feed the Arduino directly from Loconet power. A good DC/DC converter can reduce the power consumption up to 50%, however a bad one hardly has effect. See https://forum.beneluxspoor.net/index.php?topic=13589.msg3222318435#msg3222318435 for the tested situations and see what fits your case best.
   In case you do not want to use a DC/DC converter, put a solder blob on solderings pads RAW1 to power the Arduino directly from Loconet.  
   NOTE: it is not recommended to power the Arduino with voltages above 12VDC.

## Soldering components
The easiest approach is to solder from lowest componets to highest.

3. place the 6-pole connector for the Emergency stop button on position J8. Make sure that the pins for the counter connector are on the outside.  
  ![This is an image](/images/instructions/step1a.jpeg)  
  Turn the board around and solder one pin. It is easiest to start with RD+ (GND is connected to the board plane and requires more heat to solder).  
  ![This is an image](/images/instructions/step1b.jpeg)  
  If the first pin is soldered, check if the connector presses everywhere agains the board and is straight. If not, heat up pin RD+ and correct the location.  
After this solder the rest of the pins.

4. place the arduino headers.  
  ![This is an image](/images/instructions/step2.jpeg)  
  It can be handy to use place the Arduino already in and after that solder the headers.
  ![This is an image](/images/instructions/step3.jpeg)  
  Also here, it is adviseble to first solder the top and bottom pin, check orientation, correct if needed and after that continue to solder all the other pins.  If the orientation is good, remove the Arduino, otherwise you are during soldering also heating up those pins (will still work, but takes longer). 
  
5. OPTIONAL I2C headers
   It is possible to place headers for an additional display (not supported in the code, for the tinkerers) or other sensors.
   Most people will not need this, but the headers are provided and can be soldered on J2 and J3.
  
6.  If you decided in step 2 to use a DC/DC converter, place this one as in the picture (it should fit inside the white box on the PCB).
  If you decide to use a different DC/DC converter as the XP Power VR05S05, make sure to check the datasheet. Some DC/DC converters (e.g. some Recom) have the orientation mirrored. The input pin is located closest to the Loconet connectors.
  ![This is an image](/images/instructions/step4.jpeg)  
  
7. Place the Loconet connectors and solder them. 
  ![This is an image](/images/instructions/step5.jpeg)
  
