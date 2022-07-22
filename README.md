# Loconet_Master_Switch

1. Download and install the Arduino IDE software: https://www.arduino.cc/en/software  
2. Install relevant libraries: Top bar: Tools -> Manage libraries -> 
  - search for "Loconet" and install latest version  
  - search for "Bounce2" and install latest version  
3. Download the .ino file from https://github.com/Monobrachius/Loconet_Master_Switch which contains the code for the emergency stop button.  
   At this moment I cannot find the download button, so you have to manually copy the code from: https://github.com/Monobrachius/Loconet_Master_Switch/blob/main/Loconet_Master_Switch_HK_V02.ino  
4. Select the desired options in the software
  -//Definitions  
   const int  EStopLatching              = 0;    //1 = latching (button does not fall back, but needs turning/pushing to return to "normal" position), 0 = momentary
   const int  EStopNCtype                = 0;    //1 = NC, 0 = NO
   const int  EStopOneButtonForAll       = 0;    //1 = one momentary button for set and reset, 0 for two buttons or a latching button
   const int  EStopKeptActiveWhilePushed = 0;    //1 = Keeping E-Stop while pushed, 0 = E-Stop can be reset while button is still pushed, but button will flash in that case.
   
5. Select the correct Arduino: Top bar: Tools -> Board -> select desired board.  
  - The board is tested with the following theArduino Nano & Arduino Pro Mini. Other boards might work as well, but are not tested. 
6. Select the correct processor: Top bar: Tools -> Processor -> select used processor  
  - Arduino Nano: red board is the Atmega168 (should also be mentioned when you buy it), the blue ones with USB-C are most times Atmega328P, the blue ones with Mini-USB are most times Atmega328P with old bootloader.  
  - Arduino Pro Mini: check with your supplier, there is a too wide of range to simply link it by color and port type.  

7. Press compile (left check mark ✓, just below "File" ont the Top bar), this will check if the code compiles, or that something is incorrect (most times: library missing, or wrong type of board)  
8. If you have chosen the correct board, processor and port (port should autoselect when you plug in only 1 Arduino), it should compile the code once again and after that straight away upload to the Arduino.  
