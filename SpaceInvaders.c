
#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "time.h"
#include "stdlib.h"

void PortF_Init(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;


// ------------------------- Images ------------------------------------------

// width=16 x height=10
const unsigned char car[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xF0,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};
const unsigned char enemy_car[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};

//---------------------- images -----------------------------

#define carW             ((unsigned char)car[18])
#define carH             ((unsigned char)car[22])
#define enemy_carW        ((unsigned char)car[18])
#define enemy_carH        ((unsigned char)car[22])
	
unsigned long SW1,SW2;                       // input from PF4,PF0
int player_x = 0, player_y ;			          //player position(x, y)
int enemy_posi ,enemyMove1, enemyMove2;		 //enemies position and movement
int track[3] = {11,28, 45};								//tracks positions
int flag1 = 0, flag2 = 0;									   //flags
int score ;

int main(void){
	
	PortF_Init();                           // Call initialization of port PF4, PF3, PF2, PF1, PF0 
	player_y = 28;		                     //the position of the player at the mid of screen (y = 28)
	enemyMove1 = 84-enemy_carW;
	enemyMove2 = 84-enemy_carW;
	enemy_posi = rand()%3;                 // generate a random track for the enenmy car
	score = 0;
	
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(1);
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      
	
//---------------------------- start screen-------------------------------------
	Nokia5110_Clear();
	Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("Race Game");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Ready!");
  Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("press SW1");
  Nokia5110_SetCursor(1, 4);
  Nokia5110_OutString("to play");
  
	while(1){
		  SW1 = GPIO_PORTF_DATA_R&0x10;                   // read PF4 into SW1
		if(!SW1)
			break;
  }
	
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(player_x, player_y, car, 0);
	Nokia5110_DisplayBuffer();                        // display the player car
	Delay100ms(100);
//--------------------------------------------------------------------------
	while(1){
		Nokia5110_ClearBuffer();		
	  SW1 = GPIO_PORTF_DATA_R&0x10;                   // read PF4 into SW1
    SW2 = GPIO_PORTF_DATA_R&0x01;                   // read PF0 into SW2
		if(!(SW1&&SW2 )){
			if(!SW1){                                    // if SW1 is pressesd
				if((player_y > 11) && (flag1 == 0)){
					player_y -= 17;                         // move up
					flag1 = 1;
				}
			}
			else{
				flag1 = 0;
			}

			if(!SW2){                                    // if SW2 is pressed
				if((player_y < 45) && (flag2 == 0)){
					player_y += 17;                         // move down
					flag2 = 1;
				}
			}
			else{
				flag2 = 0;
			}
		}
		
		Nokia5110_PrintBMP(player_x, player_y, car, 0);
		
//---------------------------------------------------------------------------
		if(enemyMove1 != 0){
			Nokia5110_PrintBMP(enemyMove1, track[enemy_posi], enemy_car, 0);
			enemyMove1--;                                // the enemy movement 
		
		}
		
		if(enemyMove1 == 0){
			
			if(enemyMove2 == 0){
				score+=10;					                   //score =The number of passed enemys
				enemy_posi = rand()%3;
				enemyMove2 = 84-enemy_carW;
			}
			Nokia5110_PrintBMP(enemyMove2, track[enemy_posi],enemy_car, 0);
		}
		
		if(enemyMove2 == player_x+carW-2 && track[enemy_posi] == player_y){  //if the enemy hit the player car the game is over
			break;
		}
		enemyMove2--;
		
		Nokia5110_DisplayBuffer();
		Delay100ms(1);
	}
			

//--------------------- GameOver screen --------------------
  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 0);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("Nice try");
	Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("score=");
	Nokia5110_SetCursor(1, 3);
  Nokia5110_OutUDec(score);
 
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
}

void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240/80;  // 0.1sec at 80MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
