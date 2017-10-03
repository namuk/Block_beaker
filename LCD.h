/*
 * LCD.h
 *
 * Created: 2017-03-30 오후 2:05:46
 *  Author: 316-16
 */ 


#ifndef LCD_H_
#define LCD_H_

#define LCD_WDATA PORTA		// LCD 데이터버스정의(데이터쓰기)
#define LCD_WINST PORTA		// LCD 데이터버스정의(명령어쓰기)
#define LCD_RDATA PINA		// LCD 데이터버스정의(데이터읽기)

#define LCD_CTRL PORTG		// LCD 제어신호정의
#define LCD_EN 0			// Enable 신호
#define LCD_RW 1			// 읽기(1) / 쓰기(0)
#define LCD_RS 2			// 데이터(1) / 명령어(0)

#define RIGHT 1
#define LEFT 0

typedef unsigned int Word;
typedef unsigned char Byte;

#define sei() asm("sei")
#define cli() asm("cli")

void LCD_Data(Byte ch);								// LCD_DR에데이터출력
void LCD_Comm(Byte ch);								// LCD IR에명령어쓰기
void LCD_CHAR(Byte c);								// 한문자출력
void LCD_STR(Byte *str);							// 문자열출력
void LCD_pos(unsigned char row, unsigned char col);	//LCD 표시 위치 지정
void LCD_Clear();									//화면 클리어
void LCD_Init(void);								// LCD 초기화
void Display_Shift(char direction);
void Cursor_Shift(char direction);
void Cursor_Home();									//커서의 위치를 화면의 처음으로 설정

#endif /* LCD_H_ */