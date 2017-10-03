/*
 * LED.c
 *
 * Created: 2017-03-23 오후 3:29:54
 *  Author: 316-16
 */ 

#define F_CPU 14745600UL		
#include <avr/io.h>				
#include <util/delay.h>

typedef unsigned char Byte;
Byte count;

void PB_LShift()
{
	int i,j;
	Byte LED = 0xFE;
	
	for(i=0; i<8;i++)
	{
		for(j=0;j<500;j++)
		{
			_delay_ms(1);
		}
		PORTB = LED;
		LED = (LED<<1)|0x01;
	}
}

void PB_RShift()
{
	int i,j;
	Byte LED = 0x7F;
	
	for(i=0; i<8;i++)
	{
		for(j=0;j<500;j++)
		{
			_delay_ms(1);
		}
		PORTB = LED;
		LED = (LED>>1)|0x80;
	}
}

void PB_Flash()
{
	Byte LED = 0xFF;
	int i,j;
	
	for(i=0; i<8;i++)
	{
		PORTB = LED;
		for(j=0;j<500;j++)
		{
			_delay_ms(1);
		}
		LED = ~LED;
	}
}

void PB_Toggle()
{
	int i,j;
	Byte LED = 0xF0;
	
	for(i=0; i<8;i++)
	{
		PORTB = LED;
		for(j=0;j<500;j++)
		{
			_delay_ms(1);
		}
		LED = ~LED;
	}
}