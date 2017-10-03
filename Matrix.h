/*
 * Matrix.h
 *
 * Created: 2017-05-26 오후 4:36:25
 *  Author: 316-16
 */ 


#ifndef MATRIX_H_
#define MATRIX_H_

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

#define CLK_HIGH()  PORTB |= (1<<PB2)
#define CLK_LOW()   PORTB &= ~(1<<PB2)
#define CS_HIGH()   PORTB |= (1<<PB1)
#define CS_LOW()    PORTB &= ~(1<<PB1)
#define DATA_HIGH() PORTB |= (1<<PB0)
#define DATA_LOW()  PORTB &= ~(1<<PB0)
#define INIT_PORT() DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2)

uint8_t display[8];

void spi_send(uint8_t data);
void max7219_writec(uint8_t high_byte, uint8_t low_byte);
void max7219_clear(void);
void max7219_init(void);
void update_display(void);
void image(const __flash uint8_t im[8]);
void set_pixel(uint8_t r, uint8_t c, uint8_t value);

#endif /* MATRIX_H_ */