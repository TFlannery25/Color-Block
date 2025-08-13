# Color-Block

## Introduction
Color block is a 2D color-matching game where you control a moving square. As walls approach, you must align your cube with the section of the wall that matches its color. You score  points for every wall you successfully pass through. The game ends if you hit a section with the wrong color. 

The player chooses to play either the regular mode or the infinite mode using the direction of the potentiometer at the beginning. The player then controls the cube’s left and right movement using the potentiometer, where the rotation amount increases the cube’s acceleration. Walls move downward toward the player, and the goal is to steer the cube through the section that matches its color. Each successful wall passed earns 10 points. Reaching 100 points wins the game in the regular mode, triggering green lights and a victory message. Colliding with the wrong color ends the game, flashing red lights and displaying a loss message.

A feature that was not implemented yet is a setting to allow the player to change the speed of the wall. Two bugs in the game are, once a high enough score is achieved in the infinite mode then there is overflow on the score value, and after an extended period of playing the timing of the game events deviate from its core functionality.

## User Guide
The gameplay of the cube and walls is displayed on HiLetgo 1.44" Colorful SPI TFT LCD Display. The score and other messages to the player are displayed on the LCD1602 Module. Sound effects are generated on a passive buzzer. Lights are produced by a RGB LED. User input is taken through a potentiometer to select the game mode at the beginning. Moving to the right is the regular mode and to left is infinite mode. Then the potentiometer controls the square left and right. The player must move through the green walls to score points. A button can be pressed to soft reset the game.

## Hardware Components Used

### Computing:

•	Elegoo Uno R3 Controller Board

### Inputs:

•	Potentiometer 

•	Button

### Outputs:

•	HiLetgo 1.44" Colorful SPI TFT LCD Display

•	LCD1602 Module

•	RGB LED

•	Passive Buzzer

### Software Libraries Used

•	avr/io.h

-	used for access to the registers and definitions of the AVR microcontroller.
  
•	avr/interrupt.h

-	used for timerISR to facilitate the tick functions and periods of tasks
  
•	stdlib.h

-	used for rand() and srand() functions to generate random numbers
  
•	util/delay.h

-	used to implement software delays, specifically in LCDHelper.h and TFTHelper.h

## Wiring Diagram
 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/adae8172-1278-461f-8105-1713862915c9" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/36c19860-6b38-4d3b-8153-51a866d749a8" />

## Task Diagram
<img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/a2346dac-176f-4437-a02a-258fe0c1a4ac" />

 
## SynchSM Diagrams
 
 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/9108ca81-bf4e-427d-95ac-3cd3a15d024c" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/0e81f3fd-e658-4155-b40c-e08c2a290cde" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/6daefc31-9f76-4c93-b1e9-c3c2bb9f83c7" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/2cf22b12-3e84-4d7b-9a07-d26601f839dd" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/cec031e0-eb2b-4658-bd1c-ca0b8dc92462" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/756076b6-0e8a-42ee-9605-c497293318f3" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/8451aacb-dabf-49ca-bc4e-7cfcc845d7e1" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/df5a578a-52c4-4648-9267-cb0898e5e097" />

 <img width="700" height="500" alt="image" src="https://github.com/user-attachments/assets/23a0939b-f431-4b33-a948-60f7f3543ff0" />

 <img width="909" height="475" alt="image" src="https://github.com/user-attachments/assets/ac9fa8d4-1cd1-4685-a9cb-0b96bd29136a" />

<img width="822" height="368" alt="image" src="https://github.com/user-attachments/assets/e8fa734b-415d-48f3-973f-feb5d5ef4258" />


Note: serialATmega.h and timerISR.h are class provided files
