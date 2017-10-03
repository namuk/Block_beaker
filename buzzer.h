/*
 * buzzer.h
 *
 * Created: 2017-04-27 오후 12:39:37
 *  Author: 316-16
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#define BIT(x) (1<<(x))
#define SETBIT(x,y) (x |= y)
#define CLEARBIT(x,y) (x &= ~y)
#define LDo 395
#define LRe 354
#define LMi 316
#define LFa 298
#define LSo 266
#define LLa 237
#define LSi 212
#define BDo 200
#define BRe 178
#define BMi 159
#define BFa 150
#define BSo 134
#define BLa 112
#define BSi 107
#define HDo 101
#define HRe 90
#define HMi 80
#define HFa 76
#define HSo 68
#define HLa 61
#define HSi 54
#define hDo 51
#define RB 50
#define LB 65
#define SB 30
#define BB 35
#define Buzzer_Port PORTG
#define Buzzer_Pin 4
#define MelOn SETBIT(Buzzer_Port, BIT(Buzzer_Pin))
#define MelOff CLEARBIT(Buzzer_Port,BIT(Buzzer_Pin))

void delay_us_Melody(int data);
void Sound(unsigned int ch, unsigned int time);
void S_Good();
void S_Error();
void S_Push();
void S_Start();
void S_S1();
void S_S2();
void S_S3();
void S_S4();
void S_S5();
void S_S6();
void S_S7();

#endif /* BUZZER_H_ */