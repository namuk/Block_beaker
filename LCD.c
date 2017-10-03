/*
 * LCD.c
 *
 * Created: 2017-03-30 오후 1:13:08
 *  Author: 316-16
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

void LCD_Data(Byte ch)			// LCD_DR에데이터출력
{
	LCD_CTRL |= (1<< LCD_RS) ;	// PORTG RS핀에1출력
	LCD_CTRL &= ~(1<< LCD_RW) ;	// PORTG RW핀에0출력
	LCD_CTRL |= (1<< LCD_EN) ;	// PORTG EN핀에1출력(Enable)
	_delay_us(50);				// 시간지연
	LCD_WDATA = ch;				// 데이터출력
	_delay_us(50);				// 시간지연
	LCD_CTRL &= ~(1<< LCD_EN) ; // PORTG EN핀에0출력(Disable)
}


void LCD_Comm(Byte ch)			// LCD IR에명령어쓰기
{
	LCD_CTRL &= ~(1<< LCD_RS);	// PORTG RS핀에0출력
	LCD_CTRL &= ~(1<< LCD_RW);	// PORTG RW핀에0출력
	LCD_CTRL |= (1<< LCD_EN);	// PORTG EN핀에1출력(Enable)
	_delay_us(50);				// 시간지연
	LCD_WINST = ch;				// 명령어출력
	_delay_us(50);				// 시간지연
	LCD_CTRL &= ~(1<< LCD_EN);	// PORTG EN핀에0출력(Disable)
}

void LCD_CHAR(Byte c)			// 한문자출력
{
	LCD_Data(c);
	//_delay_ms(1);
}

void LCD_STR(Byte *str)			// 문자열출력
{
	while(*str!= 0)
	{
		LCD_CHAR(*str);
		str++;
	}
}

void LCD_pos(unsigned char row, unsigned char col)	//LCD 표시 위치 지정
{
	LCD_Comm(0x80|(col+row*0x40));					// row = 문자행, col = 문자열
}

void LCD_Clear()	//화면 클리어
{
	LCD_Comm(0x01);
	_delay_ms(2);
}

void LCD_Init(void)		// LCD 초기화
{
	LCD_Comm(0x38);		// DDRAM, 데이터8비트사용, LCD 2열로사용
	_delay_ms(2);		// 2ms 지연
	LCD_Comm(0x38);		// DDRAM, 데이터8비트사용, LCD 2열로사용
	_delay_ms(2);		// 2ms 지연
	LCD_Comm(0x38);		// DDRAM, 데이터8비트사용, LCD 2열로사용
	_delay_ms(2);		// 2ms 지연
	LCD_Comm(0x0c);		// Display ON/OFF
	_delay_ms(2);		// 2ms 지연
	LCD_Comm(0x06);		// 주소+1 , 커서를우측이동
	_delay_ms(2);		// 2ms 지연
	LCD_Clear();		// LCD 화면클리어
}

void Display_Shift(char direction)
{
	if (direction == RIGHT)			// 표시화면전체를오른쪽으로이동
	{
		LCD_Comm(0x1C);
		_delay_ms(1);				// 시간지연
	}
	else if (direction == LEFT)		// 표시화면전체를왼쪽으로이동
	{
		LCD_Comm(0x18);
		_delay_ms(1);				// 시간지연
	}
}

void Cursor_Shift(char direction)
{
	if (direction == RIGHT)			// 커서를오른쪽으로이동
	{ 
		LCD_Comm(0x14);
		_delay_ms(1);				// 시간지연
	} 
	else if (direction == LEFT)		// 커서를왼쪽으로이동
	{ 
		LCD_Comm(0x10);
		_delay_ms(1);				// 시간지연
	}
}

void Cursor_Home()		//커서의 위치를 화면의 처음으로 설정
{
	LCD_Comm(0x02);		//Cursor Home
	_delay_ms(2);		
}