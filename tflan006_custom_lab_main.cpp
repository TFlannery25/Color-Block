/* 
Using class provided timerISR.h and serialATmega.h

Demo Video: https://youtu.be/S0i3sh4wQ30?si=iQnBkm5RCvg-PdxV
*/


#include <avr/interrupt.h>
#include <avr/io.h>
#include "timerISR.h"
#include "serialATmega.h"
#include "LCDHelper.h"
#include "TFTHelper.h"
#include <stdlib.h>  


#define NUM_TASKS 11 //TODO: Change to the number of tasks being used
//Task struct for concurrent synchSMs implmentations
typedef struct _task{
signed char state; //Task's current state
unsigned long period; //Task period
unsigned long elapsedTime; //Time elapsed since last task tick
int (*TickFct)(int); //Task tick function
} task;

const unsigned long LCD_PERIOD = 1;
const unsigned long Potentiometer_PERIOD = 100;
const unsigned long RGB_PERIOD = 25;
const unsigned long CUBE_PERIOD = 2;
const unsigned long SCORE_PERIOD = 1200;
const unsigned long WALL_PERIOD = 50;
const unsigned long RAND_PERIOD = 1195;
const unsigned long CHECK_PERIOD = 1200;
const unsigned long MENU_PERIOD = 100;
const unsigned long BUZZER_PERIOD = 200;
const unsigned long WIN_PERIOD = 60;
const unsigned long GCD_PERIOD = 1;
task tasks[NUM_TASKS]; 


void TimerISR() {
for ( unsigned int i = 0; i < NUM_TASKS; i++ ) { // Iterate through each task in the task array
if ( tasks[i].elapsedTime == tasks[i].period ) { // Check if the task is ready to tick
tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
tasks[i].elapsedTime = 0; // Reset the elapsed time for the next tick
}
tasks[i].elapsedTime += GCD_PERIOD; // Increment the elapsed time by GCD_PERIOD
}
}

void ADC_init() {
  // TODO: figure out register values
  ADMUX = 0b01000000/*insert your value for ADMUX*/;
  ADCSRA = 0b10000111/*insert your value for ADCSRA*/;
  ADCSRB = 0b01000000/*insert your value for ADCSRB*/;
  }
  
  
  
  unsigned int ADC_read(unsigned char chnl){
  // ^^^^ unsigned char chnl selects which pin
  //you're reading analog signal from
  ADMUX = (ADMUX & 0xF0) | (chnl & 0x0F) /*set MUX3:0 bits without modifying any other bits*/;
  ADCSRA = ADCSRA | (0x01 << 6);/*set the bit that starts conversion in free running mode without
  modifying any other bit*/
  while((ADCSRA >> 6)&0x01) {} /*what does this line do?*/
  ADCSRA |= (1 << ADIF);
  uint8_t low, high;
  low = ADCL /*what should this get assigned with?*/;
  high = ADCH /*what should this get assigned with?*/;
  return ((high << 8) | low) ;
  }



enum LDC_States {LCD_Start, LCD_Score, LCD_Win, LCD_Lose};
int TickFct_LCD(int state);

enum Potentiometer_States {Potentiometer_Start, Potentiometer_Read};
int TickFct_Potentiometer(int state);

enum RGB_States {RGB_Start, RGB_Off, RGB_Green, RGB_Red};
int TickFct_RGB(int state);

enum Cube_States {Cube_Start, Cube_Draw, Cube_Move, Cube_Clear, Cube_Off};
int TickFct_Cube(int state);

enum Score_States {Score_Start, Score_Score};
int TickFct_Score(int state);

enum Wall_States {Wall_Start, Wall_Draw, Wall_Move, Wall_Clear, Wall_Win, Wall_Lose};
int TickFct_Wall(int state);

enum Rand_States {Rand_Start, Rand_Rand};
int TickFct_Rand(int state);

enum Check_States {Check_Start, Check_Check};
int TickFct_Check(int state);

enum Menu_States {Menu_Start, Menu_Menu};
int TickFct_Menu(int state);

enum Buzzer_States {Buzzer_Start, Buzzer_Wait, Buzzer_Lose, Buzzer_Win, Buzzer_Done};
int TickFct_Buzzer(int state);

enum Win_States {Win_Start, Win_Wait, Win_Win, Win_Done};
int TickFct_Win(int state);


int main(void) {

DDRB = 0xFF;  
DDRD = 0xFF;
DDRC = 0x0B;

PORTB = 0x00;
PORTD = 0x00;
PORTC = 0x00;

tasks[0].period = LCD_PERIOD;
tasks[0].elapsedTime = LCD_PERIOD;
tasks[0].state = LCD_Start;
tasks[0].TickFct = &TickFct_LCD;

tasks[1].period = Potentiometer_PERIOD;
tasks[1].elapsedTime = Potentiometer_PERIOD;
tasks[1].state = Potentiometer_Start;
tasks[1].TickFct = &TickFct_Potentiometer;

tasks[2].period = RGB_PERIOD;
tasks[2].elapsedTime = RGB_PERIOD;
tasks[2].state = RGB_Start;
tasks[2].TickFct = &TickFct_RGB;

tasks[3].period = CUBE_PERIOD;
tasks[3].elapsedTime = CUBE_PERIOD;
tasks[3].state = Cube_Start;
tasks[3].TickFct = &TickFct_Cube;

tasks[4].period = SCORE_PERIOD;
tasks[4].elapsedTime = 0;
tasks[4].state = Score_Start;
tasks[4].TickFct = &TickFct_Score;

tasks[5].period = WALL_PERIOD;
tasks[5].elapsedTime = WALL_PERIOD;
tasks[5].state = Wall_Start;
tasks[5].TickFct = &TickFct_Wall;

tasks[6].period = RAND_PERIOD;
tasks[6].elapsedTime = RAND_PERIOD;
tasks[6].state = Rand_Start;
tasks[6].TickFct = &TickFct_Rand;

tasks[7].period = CHECK_PERIOD;
tasks[7].elapsedTime = 200;
tasks[7].state = Check_Start;
tasks[7].TickFct = &TickFct_Check;

tasks[8].period = MENU_PERIOD;
tasks[8].elapsedTime = MENU_PERIOD;
tasks[8].state = Menu_Start;
tasks[8].TickFct = &TickFct_Menu;

tasks[9].period = BUZZER_PERIOD;
tasks[9].elapsedTime = BUZZER_PERIOD;
tasks[9].state = Buzzer_Start;
tasks[9].TickFct = &TickFct_Buzzer;

tasks[10].period = WIN_PERIOD;
tasks[10].elapsedTime = WIN_PERIOD;
tasks[10].state = Win_Start;
tasks[10].TickFct = &TickFct_Win;

ADC_init(); 
serial_init(9600);
lcd_Init();
TFT_Init();

//DDRD |= (1 << PD3);
//TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);

//TCCR2B = (TCCR2B & 0xF8) | 0x04;

TCCR0A |= (1 << COM0A1);// use Channel A
TCCR0A |= (1 << WGM01) | (1 << WGM00);// set fast PWM Mode

TCCR0B = (TCCR0B & 0xF8) | 0x05;
OCR0A = 255;


TimerSet(GCD_PERIOD);
TimerOn();
while (1) {}
      

      
return 0;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t smile[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

uint8_t sad[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b01110,
  0b10001,
  0b00000,
  0b00000
};

unsigned char pot_pos;
unsigned char score = 0;
unsigned char wall_Y;
unsigned char safeZone;
unsigned char check = 0;
bool startGame = 0;
bool regular;
bool winDone = 0;

int TickFct_LCD(int state)
{
  switch (state) {
    case LCD_Start:
      lcdCreateChar(0, smile);
      lcdCreateChar(1, sad);
      if(startGame == 1)
      {state = LCD_Score;}
      break;

    case LCD_Score:
      if(score == 100 && regular)
      {
        state = LCD_Win;
      }
      else if(check)
      {
        state = LCD_Lose;
      }
      else 
      {
        state = LCD_Score;
      }
      break;

    case LCD_Win:
      state = LCD_Win;
      break;
    
    case LCD_Lose:
      state = LCD_Lose;
      break;
  }

  switch (state) {
    case LCD_Start:
      break;

    case LCD_Score:
      lcdSetCursor(0,0);
      lcdPrintString("Score:");
      lcdPrint(score);
      break;

    case LCD_Win:
      lcdSetCursor(0, 0);
      lcdPrintString("You Win!");
      lcdSetCursor(0, 1);
      lcdWrite(0);
      lcdSetCursor(1, 1);
      lcdWrite(0);
      lcdSetCursor(2, 1);
      lcdWrite(0);
      break; 

      case LCD_Lose:
      lcdSetCursor(0, 0);
      lcdPrintString("You Lose!");
      lcdSetCursor(0, 1);
      lcdWrite(1);
      lcdSetCursor(1, 1);
      lcdWrite(1);
      lcdSetCursor(2, 1);
      lcdWrite(1);
      break;
  }
  return state;
}

int TickFct_Potentiometer(int state)
{
  switch(state)
  {
    case Potentiometer_Start:
      state = Potentiometer_Read;
      break;

    case Potentiometer_Read:
      state = Potentiometer_Read;
      break;
  }

  switch(state)
  {
    case Potentiometer_Start:
      break;

    case Potentiometer_Read:
      pot_pos = map(ADC_read(5),0,1023,4,112);
      break;
  }
  return state;
}

int TickFct_RGB(int state)
{
  switch(state)
  {
    case RGB_Start:
      state = RGB_Off;
      break;

    case RGB_Off:
      if(score == 100 && regular)
      {
        state = RGB_Green;
      }
      else if(check == 1)
      {
        state = RGB_Red;
      }
      else 
      {
        state = RGB_Off;
      }
      break;

    case RGB_Green:
      break;

    case RGB_Red:
      break;
  }
  switch(state)
  {
    case RGB_Start:
      break;

    case RGB_Off:
      break;

    case RGB_Green:
      PORTC ^= (1 << PC0);
      break;

    case RGB_Red:
      PORTC ^= (1 << PC1);
      break;
  }

  return state;
}

int TickFct_Cube(int state)
{
  static unsigned char old_pos;
  switch(state)
  {
    case Cube_Start:
      old_pos = pot_pos;
      if(startGame == 1)
      {
        state = Cube_Draw;
      }
      break;

    case Cube_Move:
      if(check || (score == 100 && regular))
      {
        state = Cube_Off;
      }
      else if (pot_pos != old_pos)
      {
        state = Cube_Clear;
      }
      break;

    case Cube_Clear:
      state = Cube_Draw;
      break;

    case Cube_Draw:
      old_pos = pot_pos;  
      state = Cube_Move;
      break;

    case Cube_Off:
      break;
  }


  switch(state)
  {
    case Cube_Start:
      break;

    case Cube_Draw:
      TFTRectangle(pot_pos, 16, 16, 16, 0x07E0);  
      break;

    case Cube_Move:
      TFTRectangle(pot_pos, 16, 16, 16, 0x07E0);
      break;

    case Cube_Clear:
      TFTRectangle(old_pos, 16, 16, 16, 0x0000);
      break;

    case Cube_Off:
      break;
  }

  return state;
}

int TickFct_Score(int state)
{
  switch(state)
  {
      case Score_Start:
        if(startGame == 1)
        {
          state = Score_Score;
        }
        break;

      case Score_Score:
        break;
  }

  switch(state)
  {
      case Score_Start:
        break;

      case Score_Score:
        score += 20;
        break;
  }
    return state;
}

int TickFct_Wall(int state)
{
  static unsigned char old_wall_y;
  switch(state)
  {
    case Wall_Start:
      old_wall_y = wall_Y = 112;
      if(startGame == 1)
      {
        state = Wall_Draw;
      }
      break;

    case Wall_Move:
      if (wall_Y != old_wall_y)
      {
        state = Wall_Clear;
      }
      break;

    case Wall_Clear:
      if(score == 100 && regular)
      {
        state = Wall_Win;
      }
      else if(check)
      {
        state = Wall_Lose;
      }
      else{
      state = Wall_Draw;}
      break;

    case Wall_Draw:
      old_wall_y = wall_Y;  
      state = Wall_Move;
      break;
  }

  switch(state)
  {
    case Wall_Start:
      break;

    case Wall_Draw:
      TFTRectangle(0, wall_Y, 128, 16, 0x001F);  
      if (safeZone == 0)
      {
        TFTRectangle(0, wall_Y, 42, 16, 0x07E0);  
      }
      if (safeZone == 1)
      {
        TFTRectangle(43, wall_Y, 43, 16, 0x07E0);  
      }
      if (safeZone == 2)
      {
        TFTRectangle(86, wall_Y, 42, 16, 0x07E0);  
      }
      
      break;

    case Wall_Move:
       if (wall_Y > 0) {
        wall_Y -= 16;  
      } else {
        wall_Y = 112;  
      }
      break;

    case Wall_Clear:
      TFTRectangle(0, old_wall_y, 128, 16, 0x0000);
      break;

    case Wall_Win:
      TFTRectangle(0, 0, 130, 130, 0x07E0);
      break;

    case Wall_Lose:
      TFTRectangle(0, 0, 130, 130, 0x001F);
      break;
  }


  return state;
}

int TickFct_Rand(int state)
{
  switch(state)
  {
    case Rand_Start:
      srand(pot_pos);
      if(startGame == 1)
      {
        state = Rand_Rand;
      }
      break;

    case Rand_Rand:
      break;
  }

  switch(state)
  {
    case Rand_Start:
      break;

    case Rand_Rand:
      safeZone = rand() % 3;
      break;
  }
  return state;
}

int TickFct_Check(int state)
{
  switch(state)
  {
    case Check_Start:
      if(startGame == 1)
      {
        state = Check_Check;
      }
      break;

    case Check_Check:
      state = Check_Check;
      break;
  }
  

  switch(state)
  {
    case Check_Start:
      check = 0;
      break;

    case Check_Check:
      if (safeZone == 0)
      {
         if (pot_pos > 42)
         {
            check = 1;
         }
      }
      if (safeZone == 1)
      {
        if (pot_pos < 42 || pot_pos > 85)
         {
            check = 1;
         }
      }
      if (safeZone == 2)
      {
        if (pot_pos < 86)
        {
            check = 1;
        }
      }
      
      break;
  }

  return state;
}

int TickFct_Menu(int state)
{
  static unsigned char i;
  switch(state)
  {
    case Menu_Start:
      i = 0;
      state = Menu_Menu;
      break;

    case Menu_Menu:
      state = Menu_Menu;
      break;

  }

  switch(state)
  {
    case Menu_Start:
      state = Menu_Menu;
      break;

    case Menu_Menu:
      if(i<24)
      {
        lcdSetCursor(0,0);
        lcdPrintString("Select Mode:");
        lcdSetCursor(0,1);
        if(pot_pos < 56 )
        {
          lcdPrintString("Regular ");
        }
        else
        {
          lcdPrintString("Infinite");
        }
        startGame = 0;
        i++;
      }
      else
      {
        lcdSetCursor(0,0);
        lcdPrintString("            ");
        lcdSetCursor(0,1);
        lcdPrintString("            ");
        if(pot_pos < 56 && startGame == 0)
        {
          regular = 1;
        }
        else if (pot_pos > 56 && startGame == 0)
        {
          regular = 0;
        }
        startGame = 1;
      }
    
      break;

  }

  return state;
}

int TickFct_Buzzer(int state)
{
  static unsigned char i;
  switch(state)
  {
    case Buzzer_Start:
      i = 0;
      if(startGame)
      {
        state = Buzzer_Wait;
      }
      break;

    case Buzzer_Wait:
      if(check)
      {
        state = Buzzer_Lose;
      }
      else if(score == 100 && regular)
      {
        state = Buzzer_Win;
      }
      else 
      {
        state = Buzzer_Wait;
      }
      break;

    case Buzzer_Lose:
      state = Buzzer_Done;
      break;

    case Buzzer_Win:
      if(winDone)
      {
        state = Buzzer_Done;
      }
      break;

    case Buzzer_Done:
      state = Buzzer_Done;
      break;

  }
  switch(state)
  {
    case Buzzer_Start:
      break;

    case Buzzer_Wait:
      i++;
      if(i > 5)
      {
        i = 0;
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
        OCR0A = 128;
      }
      else
      {
        OCR0A = 255;
      }
      break;

    case Buzzer_Lose:
      TCCR0B = (TCCR0B & 0xF8) | 0x05;
      OCR0A = 128;
      break;

    case Buzzer_Win:
      break;

    case Buzzer_Done:
      OCR0A = 255;
      break;

  }
  return state;
}

int TickFct_Win(int state)
{
  static unsigned char i;
  switch(state)
  {
    case Win_Start:
      i = 0;
      state = Win_Wait;
      break;

    case Win_Wait:
      if(score == 100 && regular)
      {
        state = Win_Win;
      }
      break;

    case Win_Win:
      if(i>16)
      {
        winDone = 1;
        state = Win_Done;
      }
      break;

    case Win_Done:
      break;
  }

  switch(state)
  {
    case Win_Start:
      break;

    case Win_Wait:
      break;

    case Win_Win:
      if(i == 0)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x05;
        OCR0A = 128;
      }
      else if(i == 1)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x04;
      }
       else if(i == 2)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
      }
      else if(i == 3)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x04;
      }
      else if(i == 4)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
      }
      if(i == 5)
      {
        OCR0A = 255;
      }
      else if(i == 6)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x05;
        OCR0A = 128;
      }
       else if(i == 7)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x04;
      }
      else if(i == 8)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
      }
      else if(i == 9)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x04;
      }
      else if(i == 10)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
      }
      else if(i == 11)
      {
        OCR0A = 255;
      }
      else if(i == 12)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
        OCR0A = 128;
      }
       else if(i == 13)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x04;
      }
      else if(i == 14)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x05;
      }
      else if(i == 15)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x04;
      }
      else if(i == 16)
      {
        TCCR0B = (TCCR0B & 0xF8) | 0x03;
      }
      i++;
      break;

    case Win_Done:
      break;
  }
  return state;
}



