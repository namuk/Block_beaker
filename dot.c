/*
 * dot.c
 *
 * Created: 2017-05-26 오후 4:35:10
 *  Author: 316-16
 */ 

/*
PB0 -> DIN
PB1 -> CS
PB2 -> CLK
+5 -> VCC
GND -> GND
*/

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Matrix.h"
#include "buzzer.h"
#include "LCD.h"

typedef unsigned char Byte;
Byte sw,sw2;
volatile Word cnt, cnt1;
volatile long time;
volatile Byte sec, min, hour;
long CurTime;
unsigned char key;
int s = 0, stage = 0, level = 0;

#define sei() asm("sei")
#define cli() asm("cli")

char str0[] = " Brick breaking ";
char str1[] = "Game start : SW3";
char str2[] = " Game Fail  OTL ";
char str3[] = " Restart : SW4  ";
char str4[] = " STAGE  SUCCES! ";
char str5[] = "NEXT SW3 Re SW4 ";
char sel[] =  "  SELECT STAGE  ";
char str6[] = "back S3  NEXT S4";
char c_Time[] = " TIME  00:00:00  ";
char HP[] = "HP: ";
char B_Count[] = "BLOCK: ";
char null[] = "  ";

int g_BlockState[6] = {3, 2, 1, 0, 5, 4};
int g_BlockCount = 24, l_BlockCount = 24;

typedef enum _DIRECT { TOP, LEFT_TOP, LEFT_DOWN, DOWN, RIGHT_DOWN, RIGHT_TOP } DIRECT;

typedef struct _BALL 
{
	int nReady;
	int nHP;
	int nX, nY;
	DIRECT nDirect;
	uint32_t MoveTime;	
	uint32_t OldTime;	
} BALL;

typedef struct _BAR
{
	int nX[3];
	int nY;
} BAR;

typedef struct _BLOCK
{
	int nLife;
	int nX, nY;
} BLOCK;

BAR g_Bar;
BALL g_Ball;
BLOCK g_Block[64];

int g_StateTable[4][6] = {
	{ 3,  2, -1, -1, -1,  4},
	{-1,  5,  4, -1, -1, -1},
	{-1, -1,  1,  0,  5, -1},
	{-1, -1, -1, -1,  2,  1}
};


void SetBlock()
{
	int nX, nY, i =0;
	int xMax = 8, yMax = 3;
	
	while(i < g_BlockCount)
	{	
		if(stage < 2)
		{
			for(nY = stage; nY < yMax+stage; nY++)
			{
				for(nX = 0; nX < xMax; nX++)
				{
					g_Block[i].nLife = 1;
					g_Block[i].nX = nX;
					g_Block[i].nY = nY;
					i++;
				}
			}
		}		
		if(stage >= 2)
		{
			for(nY = 0; nY < yMax; nY++)
			{
				for(nX = 0; nX < xMax; nX+=2)
				{
					g_Block[i].nLife = 1;
					g_Block[i].nX = nX;
					g_Block[i].nY = nY;
					i++;
				}
			}
			l_BlockCount = 12;
		}
	}
	
	sprintf(B_Count, "BLOCK: %d",l_BlockCount);
	LCD_pos(1,15);
	LCD_STR(null);
	LCD_pos(1,7);
	LCD_STR(B_Count);
}

void Ball_init()
{
	g_Ball.nX = g_Bar.nX[1];
	g_Ball.nY = g_Bar.nY - 1;
	g_Ball.nDirect = TOP;
	g_Ball.OldTime = time;
	g_Ball.nReady = 1;
	g_Ball.MoveTime = 14;
	s = 0;
}

void Game_init()
{
	g_Bar.nY = 7;
	g_Bar.nX[0] = 2;
	g_Bar.nX[1] = 3;
	g_Bar.nX[2] = 4;
	g_Ball.nHP = 2;
	l_BlockCount = 24;
	max7219_clear();
	Ball_init();
	level = 0;
	SetBlock();
	
	
	sprintf(c_Time," TIME  00:00:00");
	LCD_pos(0,0);
	LCD_STR(c_Time);
	sprintf(HP, "HP: %d",g_Ball.nHP);
	LCD_pos(1,0);
	LCD_STR(HP);		
	sprintf(B_Count, "BLOCK: %d",l_BlockCount);
	LCD_pos(1,7);
	LCD_STR(B_Count);
}

int Collision(int nX, int nY)
{
	int i, nCount = 0, bCount = 0;
	
	for (i = 0; i < g_BlockCount; i++)
	{
		if (g_Block[i].nLife == 1)
		{
			if(g_Block[i].nY == nY)
			{
				if(g_Block[i].nX == nX)
				{
					g_Ball.nDirect = g_BlockState[g_Ball.nDirect];
					g_Block[i].nLife = 0;
					l_BlockCount--;
					bCount++;
					
					sprintf(B_Count, "BLOCK: %d",l_BlockCount);
					LCD_pos(1,15);
					LCD_STR(null);
					LCD_pos(1,7);
					LCD_STR(B_Count);
				}
			}
		}
	}
	if (bCount != 0)	return 1;
	
	for(i = 0; i < 3; i++)
	{
		if(nY == g_Bar.nY)
		{
			if(nX >= g_Bar.nX[0] && nX <= g_Bar.nX[2])
			{
				g_Ball.nDirect = g_BlockState[g_Ball.nDirect];
				return 1;
			}
		}
	}
	
	if(nY < 0)
	{
		g_Ball.nDirect = g_StateTable[0][g_Ball.nDirect];
		return 1;
	}
	if(nX > 7)
	{
		g_Ball.nDirect = g_StateTable[1][g_Ball.nDirect];
		return 1;
	}
	if(nY > 7)
	{
		g_Ball.nDirect = g_StateTable[2][g_Ball.nDirect];
		g_Ball.nHP = g_Ball.nHP - 1;
		Ball_init();
		
		sprintf(HP, "HP : %d",g_Ball.nHP);
		LCD_pos(1,0);
		LCD_STR(HP);
		return 1;
	}
	if(nX < 0)
	{
		g_Ball.nDirect = g_StateTable[3][g_Ball.nDirect];
		return 1;
	}
	return 0;
}

void Ball_Update()
{
	uint32_t CurTime = time;
	if(g_Ball.nReady == 0)
	{
		if(CurTime - g_Ball.OldTime > g_Ball.MoveTime)
		{
			g_Ball.OldTime = CurTime;
			
			switch(g_Ball.nDirect)
			{
				case TOP :
				if(Collision(g_Ball.nX, g_Ball.nY - 1) == 0)
					g_Ball.nY--;
				break;
				case LEFT_TOP :
				if(Collision(g_Ball.nX + 1, g_Ball.nY - 1) == 0)
				{
					g_Ball.nX++;
					g_Ball.nY--;
				}
				break;
			case LEFT_DOWN :
				if(Collision(g_Ball.nX + 1, g_Ball.nY + 1) == 0)
				{
					g_Ball.nX++;
					g_Ball.nY++;
				}
				break;
			case DOWN :
				if(Collision(g_Ball.nX, g_Ball.nY + 1) == 0)
				g_Ball.nY++;
				break;				
			case RIGHT_DOWN :
				if(Collision(g_Ball.nX - 1, g_Ball.nY + 1) == 0)
				{
					g_Ball.nX--;
					g_Ball.nY++;
				}
				break;
			case RIGHT_TOP :
				if(Collision(g_Ball.nX - 1, g_Ball.nY - 1) == 0)
				{
					g_Ball.nX--;
					g_Ball.nY--;
				}
				break;
			}
		}
	}
}

void Render()
{
	int i;
	
	set_pixel(g_Ball.nY, g_Ball.nX, 1);
	update_display();
	set_pixel(g_Ball.nY, g_Ball.nX, 0);
	
	for(i = 0; i < 3; i++)
	{
		set_pixel(g_Bar.nY, g_Bar.nX[i], 1);
		update_display();
		set_pixel(g_Bar.nY, g_Bar.nX[i], 0);
	}
	
	for(i = 0; i < g_BlockCount; i++)
	{
		if(g_Block[i].nLife == 1)
		{
			set_pixel(g_Block[i].nY, g_Block[i].nX, 1);
		}
		else set_pixel(g_Block[i].nY, g_Block[i].nX, 0);
	}
}

void Port_Init(void)
{
	PORTA =0x00; DDRA = 0xFF;
	PORTD =0x00; DDRD = 0x00;
	PORTG =0x00; DDRG = 0x1F;
}

void Interrupt_init()
{
	EICRA = (1<<ISC00)|(1<<ISC01)|(1<<ISC10)|(1<<ISC11);
	EIMSK = 0x03;
	DDRD = 0;
	SREG |= 0x80;
}

void Init_Timer0(void)
{
	TCCR0 |= 1<< CS01;
	TCCR0 |= (1<<WGM01);
	OCR0 = 91;
	TIMSK = (1<<OCIE0);
}

void init_devices(void)
{
	cli();
	Port_Init();
	LCD_Init();
	Interrupt_init();
	Init_Timer0();
	sei();
}

ISR(INT0_vect)
{
	sw = 1;
}

ISR(INT1_vect)
{
	sw2 = 1;
}

ISR(TIMER0_COMP_vect)
{
	cnt++;
	cnt1++;
	
	if(cnt == 200)
	{
		cnt = 0;
		time++;
	}
	
	if(cnt1 == 20000)
	{
		cnt1 = 0;
		sec++;
		if (sec >= 60) {
			min++; sec = 0;
		}
		if (min >= 60) {
			hour++; min = 0;
			
		}
		if (hour >= 24) hour = 0;
	}
}

void Display_clock()
{
	LCD_pos(0,7);
	LCD_CHAR((hour/10)+'0');
	LCD_CHAR((hour%10)+'0');
	LCD_pos(0,10);
	LCD_CHAR((min/10)+'0');
	LCD_CHAR((min%10)+'0');
	LCD_pos(0,13);
	LCD_CHAR((sec/10)+'0');
	LCD_CHAR((sec%10)+'0');
	
}

void Game_start()
{
	LCD_Clear();
	sw = 0;
	while(1)
	{
		if(sw == 1)
		{
			LCD_Clear();
			sw = 0;
			break;
		}
		
		LCD_pos(0,0);
		LCD_STR(str0);
		LCD_pos(1,0);
		LCD_STR(str1);
		
		S_Start();
	}
}

void Game_end(int state)
{
	sw2 = 0;
	LCD_Clear();
	while(1)
	{
		if(state == 0)
		{	
			if(sw2 == 1)
			{
				hour = 0; min = 0; sec = 0;
				MelOff;
				LCD_Clear();
				Game_init();
				LCD_pos(0,0);
				LCD_STR(c_Time);
				sw2 = 0;
				break;
			}
			LCD_pos(0,0);
			LCD_STR(str2);
			LCD_pos(1,0);
			LCD_STR(str3);
			
			S_S5();
		}
		if(state == 1)
		{
			if(sw2 == 1)
			{
				hour = 0; min = 0; sec = 0;
				MelOff;
				LCD_Clear();
				Game_init();
				sw2 = 0;
				break;
			}
			if(sw == 1)
			{
				stage++;
				hour = 0; min = 0; sec = 0;
				MelOff;
				LCD_Clear();
				Game_init();
				sw2 = 0;
				break;
			}
			LCD_pos(0,0);
			LCD_STR(str4);
			LCD_pos(1,0);
			LCD_STR(str5);
			
			S_S2();
		}
	}
}
void select_stage()
{
	LCD_pos(0,0);
	LCD_STR(sel);
	LCD_pos(1,0);
	LCD_STR(str6);
	
	while(1)
	{
		key = (PIND & 0xFF);
		if (sw == 1)
		{
			if (stage > 0)	stage--;
			hour = 0; min = 0; sec = 0;
			MelOff;
			LCD_Clear();
			Game_init();
			sw = 0;
		}
		if (sw2 == 1)
		{
			if (stage <= 2)	stage++;
			hour = 0; min = 0; sec = 0;
			MelOff;
			LCD_Clear();
			Game_init();
			sw2 = 0;
		}
		if(key == 0x7F)	break;
		
		Ball_Update();
		Render();
	}
}

int main(void)
{
	int i=0, nDirect = 0;
	int count = 0;
	
	init_devices();
	max7219_init();

	for (i = 0 ; i < 8*8; i++)
	{
		set_pixel(i/8, i%8, 2);	
		update_display();
		_delay_ms(50);
	}
	_delay_ms(300);

	Game_start();
	max7219_clear();

	Game_init();
	select_stage();
	
	while(1)
	{
		Display_clock();
		key = (PIND & 0xFF);	
		if(sec % 20 == 0)	g_Ball.MoveTime = 11;

		if(g_Ball.nHP == 0)	Game_end(0);
		if(l_BlockCount == 0) Game_end(1);
		if(key == 0x7F)
		{
			s++;
			g_Ball.nReady = 0;
			nDirect = 2;
			g_Ball.nDirect = nDirect;
		}
		if(s == 0)
		{
			g_Ball.nX = g_Bar.nX[1];
			g_Ball.nY = g_Bar.nY - 1;
		}
		
		if(sw == 1)
		{
			sw = 0;
			if(g_Bar.nX[0] >0)
			{
				g_Bar.nX[0]--;
				g_Bar.nX[1]--;
				g_Bar.nX[2]--;
			}
		}
		if(sw2 == 1)
		{
			sw2 = 0;
			if(g_Bar.nX[2] <= 6)
			{
				g_Bar.nX[0]++;
				g_Bar.nX[1]++;
				g_Bar.nX[2]++;
			}
		}
		
		Ball_Update();
		Render();
	}
}