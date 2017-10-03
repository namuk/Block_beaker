/*
 * Buzzer.c
 *
 * Created: 2017-04-27 오후 12:39:12
 *  Author: 316-16
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include "buzzer.h"

void delay_us_Melody(int data)
{
   int i;
   for(i=0;i<data;i++)
   {
      
      _delay_us(1);
   }
}

void Sound(unsigned int ch, unsigned int time)
{
   unsigned int tim;
   tim = ch;
   tim = tim*1000/time;
   while(tim--){
      MelOn;
      delay_us_Melody(time);
      MelOff;
      delay_us_Melody(time);
   }
}

void S_Good()
{
   Sound(BB,BDo);
   Sound(BB,BSo);
}

void S_Error(void)
{
   Sound(SB,LDo);
}

void S_Push()
{
   Sound(SB,HDo);
   Sound(SB,HMi);
}

void S_Start()
{
   Sound(BB,HDo);
   Sound(SB,HRe);
   Sound(BB,HMi);
   Sound(SB,HFa);
   Sound(BB,HSo);
   Sound(SB,HLa);
   Sound(BB,HSi);
   Sound(SB,hDo);
}

void S_S1()
{
   Sound(BB,BDo);
   Sound(BB,BMi);
   Sound(BB,BSo);
   Sound(BB,HDo);
}

void S_S2()
{
   Sound(BB,HDo);
   Sound(BB,HMi);
}

void S_S3()
{
   Sound(BB,LDo);
}

void S_S4()
{
   Sound(BB,BDo);
   Sound(BB,BMi);
   Sound(BB,BSo);
}

void S_S5()
{
   Sound(BB,BDo);
   Sound(BB,BMi);
}

void S_S6()
{
   int i;
   for(i=0; i<4; i++)
   {
      Sound(BB,HSo);
      Sound(BB,HDo);
   }
}

void S_S7()
{
   Sound(BB,BDo);
   Sound(BB,HDo);
}