# IoT-SDCard_Reader

## Project- Read Text from micro-SD card and display on LCD 
 
**Components:**  
    .. Arduino Uno  
    I2C 16*2 LCD  
    Micro-SD card Module  
    Micro-SD card  
    4 - Push Button  
    4 - 1k ohm resistors 
- Circuit Connection: 
  I2C 16*2 LCD <---> Arduino Uno 
               GND <---->GND  
               VCC<---->+5V    
               SDA<---->A4   
               SCL<---->A5  
- Micro-SD card module<----> Arduino Uno 
              GND<----->GND 
              VCC<----->+5 V 
              MISO<----->pin 12 
              MOSI <----->pin 11 
              SCK <----->pin 13 
              CS <----->pin 4 
 
 

 
- Push-Button <-----> Arduino Uno 
 
Push-button 1 <----> pin 5
Push-button 2 <----> pin 6 
Push-button 3 <----> pin 7 
Push-button 4 <----> pin 8 
 
# Push-Button Function 
By default, word/minute is set to minimum 200 and maximum 1000. 
1. Button-1 (Speed increase Function)- When this button is pressed, 50 words will be added each time. 2. Button-2 (Speed decrease Function)- When this button is pressed, 50 words will be decreased each time 3. Button-3 (Pause/Resume Function)- When this button is pressed, the text will be paused and when pressed again will resume from the same word where it was paused. 4. Button-4 (Reverse Function)- When this button is pressed, it will take 10 words back on each press and start reading again from 10 words to forward.
