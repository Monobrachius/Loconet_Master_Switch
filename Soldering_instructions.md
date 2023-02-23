# Soldering instructions (for Loconet)

1. See which lights you have. By default 5V is applied with a 330 Ohm resistor (R20 and R21) in series.  
In case your lights are not bright enough, there are three options:  
-  1) short the 330 Ohm resistors (JP1 and JP2). 
-  2) increase the voltage to the light to the Loconet Voltage by cutting the jumper on RAW_5V0 with a sharp knife (measure with a multimeter if there if the pads are really seperate) and put a solder blob between the middle pad and the pad below the RAW text. 
-  3) combine 1) and 2).

2. First decide if you want to use the a DC/DC converter (e.g. XP Power VR05S05) or to feed the Arduino directly from Loconet power. A good DC/DC converter can reduce the power consumption up to 50%, however a bad one hardly has effect. See https://forum.beneluxspoor.net/index.php?topic=13589.msg3222318435#msg3222318435 for the tested situations and see what fits your case best.

##Soldering components
The easiest approach is to solder from lowest componets to highest.

3. place the 6-pole connector for the Emergency stop button on position J8.
  ![This is an image](/images/instructions/step1a.jpeg)
  - search for "Bounce2" and install latest version  
  ![This is an image](/images/instructions/step1b.jpeg)
  ![This is an image](/images/instructions/step1b.jpeg)
  ![This is an image](/images/instructions/step2.jpeg)
  ![This is an image](/images/instructions/step3.jpeg)
  ![This is an image](/images/instructions/step4.jpeg)
  ![This is an image](/images/instructions/step5.jpeg)
  
