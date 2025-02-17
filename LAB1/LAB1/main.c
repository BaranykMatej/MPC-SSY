/*
 * LAB1.c
 *
 * Created: 02.02.2020 9:01:38
 * Author : Ondra
 */ 

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "libs/libprintfuart.h"
#include <stdio.h>
#include "libs/makra.h"
/************************************************************************/
/* DEFINES                                                              */
#define NUM 2
#define ODECET
#define sbi(var, mask)  ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)  ((var) &= (uint8_t)~(1 << mask))
#define tbi(var,mask)	(var & (1 << mask) )
#define xbi(var,mask)	((var)^=(uint8_t)(1 << mask))
	
/************************************************************************/

// F_CPU definovano primo v projektu!!! Debug->Properties->Toolchain->Symbols

/************************************************************************/
/* VARIABLES                                                            */
int a =  10;
int vysledek;
unsigned b = 255;
unsigned c = 255;
unsigned int d;
int e = 24;
int ukazovanahodnota = 333;
int *ukazatel = &ukazovanahodnota; 

/************************************************************************/

//musime vytvorit soubor pro STDOUT
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void board_init(){
	UART_init(38400); //nastaveni rychlosti UARTu, 38400b/s
	stdout = &uart_str; //presmerovani STDOUT
}

void delay_ms(int ms) {
	while (ms--) {
		_delay_ms(1);
	}
}

int main(void)
{ 	
	board_init();
	_delay_ms(1000);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //putty cls idc
	printf("Hello word\n\r");
    _delay_ms(1000);
	
/*	DDRB |= (1 << DDB6);
		    while (1)
		    {
			    PORTB &= ~(1 << PORTB6);
			    _delay_ms(500);
			    PORTB |= (1 << PORTB6);
			    _delay_ms(500);
		    }
	 		return 0;
*/

DDRB |= (1 << DDB5) | (1 << DDB6);
DDRE |= (1 << DDE3);
// nastaveni portu na ledky
while (1)
{
	PORTB = 0b00100000 | 0b01000000;
	sbi(PORTB, PB5);
	sbi(PORTB, PB6);
	sbi(PORTE, PE3);
	delay_ms(500);
	PORTB = 0b00000000;
	cbi(PORTE, PE3);
	delay_ms(500);

	PORTB = 96;
	sbi(PORTE, PE3);
	delay_ms(500);
	PORTB = 0;
	cbi(PORTE, PE3);
	delay_ms(500);

	PORTB = 0x60;
	sbi(PORTE, PE3);
	delay_ms(500);
	PORTB = 0x00;
	cbi(PORTE, PE3);
	delay_ms(500);

	sbi(PORTB, PB5);
	sbi(PORTB, PB6);
	sbi(PORTE, PE3);
	delay_ms(500);
	cbi(PORTB, PB5);
	cbi(PORTB, PB6);
	cbi(PORTE, PE3);
	delay_ms(500);

	xbi(PORTB, PB5);
	xbi(PORTB, PB6);
	xbi(PORTE, PE3);
	delay_ms(500);
}

	#ifdef ODECET
	vysledek = a - NUM;
	#endif
	printf("Odecet = %d \n\r", vysledek);
	
	d = (unsigned int)b + (unsigned int)c;
	printf("Soucet pretypovanych promennych: %u\n", d); //255+255 pretece 510 vysledek
	int posunuti = (e>>3) - 1;
	posunuti = posunuti & 0x2;
	printf("Vysledek posunuti %d\n", posunuti);
	
	int hodnota = 200;
	char str[80] = "Hodnota=";
	char hodnotaStr[20];
	itoa(hodnota, hodnotaStr, 10);
	strcat(str, hodnotaStr);
	printf("%s\n", str);
	
	int hodnota2 = 200;
	char str2[80];
	sprintf(str2, "Hodnota2=%d", hodnota2);
	printf("%s\n", str);
	
	printf("Hodnota ukazovane promenne: %d\n", *ukazatel);
	printf("Hodnota adresy pameti ukazovane promenne: %p\n", (void *)ukazatel);
	
	
	int i=0;
    while (1) 
    {
	_delay_ms(10000);
	i++;
	printf("Test x = %d \n\r", i);
    }
}

