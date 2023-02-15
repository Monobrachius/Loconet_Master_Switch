# Soldering instructions

1. See which lights you have. By default 5V is applied with a 330 Ohm resistor (R20 and R21) in series.  
In case your lights are not bright enough, there are three options:  
  1) short the 330 Ohm resistors (JP1 and JP2). 
  2) increase the voltage to the light to the Loconet Voltage by cutting the jumper on RAW_5V0 with a sharp knife (measure with a multimeter if there if the pads are really seperate) and put a solder blob between the middle pad and the pad below the RAW text. 
  3) combine 1) and 2).

2. First decide if you want to use the a DC/DC converter (e.g. XP Power VR05S05) or to feed the Arduino from the Loconet. A good DC/DC converter can reduce the power consumption up to 50%, however a bad one hardly has effect. See https://forum.beneluxspoor.net/index.php?topic=13589.msg3222318435#msg3222318435 for some example and see what fits your case best.

2. Install relevant libraries: Top bar: Tools -> Manage libraries -> 
  - search for "Loconet" and install latest version  
  ![This is an image](/images/instructions/step1a.jpeg)
  - search for "Bounce2" and install latest version  
  ![This is an image](/images/instructions/Find_Bounce2_library.PNG)
  
