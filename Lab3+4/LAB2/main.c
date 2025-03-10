//LIBS
#include "libs/macros.h"
#include "libs/i2c.h"
#include "libs/at30tse758.h"
#include "libs/adc.h"
#include "libs/libprintfuart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

//DEFINE
#define CONST 2
#define ODECET
#define UPPER_CASE 1
#define NORMAL_CASE 2
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define PRESCALE_VALUE 1024
#define PRESCALE 5
#define FREQ 2

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

int vysledek = 10;
uint16_t light; 
char out_str[30];
unsigned char uch1 = 255;
unsigned char uch2 = 255;
bool btn_pressed = false;
bool light_toggle = false;



void Timer1_cmp_start(uint16_t porovnani) {
	cli();
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 = 0;
	OCR1A = porovnani;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= 5;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1A |= (1 << COM1A0);
	sei();
}

void Timer2_fastpwm_start(uint8_t duty_cycle) {
	cli();
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	OCR2A = (255 * duty_cycle) / 100;
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22);
	TCCR2B |= (1 << CS20) | (1 << CS22);
	TCCR2A |= (1 << COM2A1);
	sei();
}

ISR(TIMER1_COMPA_vect) {
	LED1CHANGE;
}

void Timer0_ovf_start() {
	cli();
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
	TCCR0B |= PRESCALE;
	TCCR0A |= (1 << COM0A0);
	TIMSK0 |= (1 << TOIE0);
	sei();
}

ISR(TIMER0_COMPA_vect) {
    if (light_toggle) {
        // Read the light intensity from ADC
        light = ADC_get(3);  // Assuming ADC_get(3) gives the light intensity
        sprintf(out_str, "Light intensity is: %d\r\n", light);
        UART_SendString(out_str);
    }
}

// Function to start Timer0 to trigger every 100ms
void Timer0_ovf_start_100ms() {
    cli();  // Disable global interrupts
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;
    TCCR0B |= PRESCALE;  // Set prescaler for Timer0 (already defined as 5)
    TCCR0A |= (1 << COM0A0);
    TIMSK0 |= (1 << OCIE0A);  // Enable interrupt on compare match A
    OCR0A = (F_CPU / 256 / 10) - 1;  // Timer triggers every 100ms (assuming 16 MHz clock)
    sei();  // Enable global interrupts
}

// Function to stop Timer0
void Timer0_Stop() {
    TCCR0B = 0;  // Stop Timer0
    TIMSK0 = 0;  // Disable interrupts
}

void Timer1Stop() {
	TCCR1B = 0;
}

void Timer2Stop() {
	TCCR2B = 0;
	TIMSK2 = 0;
}

ISR(INT5_vect) {
	btn_pressed = true;
}

char abeceda[26];

void generateField(int case_type) {
	int i;
	if (case_type == UPPER_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = 'A' + i;
		}
		} else if (case_type == NORMAL_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = 'a' + i;
		}
		} else {
		printf("PROGRAM ERROR\n");
		return;
	}
}

void capsLetters(int case_type) {
	int i;
	if (case_type == UPPER_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = toupper(abeceda[i]);
		}
		} else if (case_type == NORMAL_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = tolower(abeceda[i]);
		}
		} else {
		printf("PROGRAM ERROR\n");
		return;
	}
}

char my_toupper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 'A';
	}
	return c;
}

char my_tolower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 'a';
	}
	return c;
}

void printField(int direction) {
	int i;
	if (direction == DIRECTION_UP) {
		for (i = 0; i < 26; i++) {
			printf("%c ", abeceda[i]);
		}
		} else if (direction == DIRECTION_DOWN) {
		for (i = 25; i >= 0; i--) {
			printf("%c ", abeceda[i]);
		}
		} else {
		printf("PROGRAM ERROR\n");
	}
}

void board_init() {
	UART_init(38400);
	stdout = &uart_str;
}

void printMenu() {
	UART_SendStringNewLine("Welcome to interactive terminal!");
	UART_SendStringNewLine("1 ...... turn on led 1");
	UART_SendStringNewLine("2 ...... turn off led 1");
	UART_SendStringNewLine("3 ...... turn on led 2");
	UART_SendStringNewLine("4 ...... turn off led 2");
	UART_SendStringNewLine("5 ...... turn on led 3");
	UART_SendStringNewLine("6 ...... turn off led 3");
	UART_SendStringNewLine("7 ...... enter button input mode");
	UART_SendStringNewLine("8 ...... turn on led blinking");
	UART_SendStringNewLine("9 ...... turn off led blinking");
	UART_SendStringNewLine("a ...... turn on PWM blinking");
	UART_SendStringNewLine("b ...... turn OFF PWM blinking");
	UART_SendStringNewLine("t ...... display temperature");
	UART_SendStringNewLine("l ...... display light intensity");
	UART_SendStringNewLine("0 ...... clear");
}

void cleanConsole() {
	for (int i = 0; i < 30; i++) {
		UART_SendStringNewLine("");
	}
}

void send_counter(int counter) {
	char buf[10];
	itoa(counter, buf, 10);
	UART_SendString("Button pressed count: ");
	UART_SendString(buf);
	UART_SendString("\r\n");
}

float readTemperature(void) {
	float temperature = at30_readTemp();
	if (temperature == 0) {
		UART_SendStringNewLine("Temperature read error\n\r");
		return -1.0;
	}
	return temperature;
}

int main(void) {
	UART_init(38400);
	sbi(DDRB, PORTB6);
	cbi(DDRE, PORTE5);
	sbi(EICRB, ISC51);
	cbi(EICRB, ISC50);
	sbi(EIMSK, INT5);
	sei();

	uint8_t test_sequence[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'U', 'A', 'R', 'T', '\r', '\n', 0 };

	// Send test sequence over UART
	for (uint8_t i = 0; test_sequence[i] != 0; i++) {
		UART_SendChar(test_sequence[i]);
	}

	// Configure LED pins and button input
	DDRB |= (1 << DDB5) | (1 << DDB6);
	DDRE |= (1 << DDE3);

	// Print the menu
	printMenu();

	uint8_t counter = 0;

	while (1) {
		char received = UART_GetChar();

		// Print the received input
		UART_SendStringNewLine("Your input is:");
		UART_SendChar(received);
		UART_SendChar('\r');
		UART_SendChar('\n');

		// Handle different user inputs
		switch (received) {
			case '0':
			cleanConsole();
			printMenu();
			break;

			case '1':
			UART_SendStringNewLine("Turning LED 1 on!");
			LED1ON;
			break;

			case '2':
			UART_SendStringNewLine("Turning LED 1 off!");
			LED1OFF;
			break;

			case '3':
			UART_SendStringNewLine("Turning LED 2 on!");
			LED2ON;
			break;

			case '4':
			UART_SendStringNewLine("Turning LED 2 off!");
			LED2OFF;
			break;

			case '5':
			UART_SendStringNewLine("Turning LED 3 on!");
			LED3ON;
			break;

			case '6':
			UART_SendStringNewLine("Turning LED 3 off!");
			LED3OFF;
			break;

			case '7':
			UART_SendStringNewLine("Entered button input mode:");
			while (!btn_pressed) {}
			send_counter(counter);
			counter++;
			btn_pressed = false;
			break;

			case '8':
			UART_SendStringNewLine("Turned on LED blinking!");
			uint16_t porovnani = (F_CPU / (2 * PRESCALE_VALUE * FREQ)) - 1;
			Timer1_cmp_start(porovnani);
			break;

			case '9':
			UART_SendStringNewLine("Turned off LED blinking!");
			Timer1Stop();
			break;

			case 'a':
			UART_SendStringNewLine("Turned on PWM LED blinking!");
			Timer2_fastpwm_start(50);
			break;

			case 'b':
			UART_SendStringNewLine("Turned off PWM LED blinking!");
			Timer2Stop();
			break;

			case 't':
			UART_SendStringNewLine("Reading temperature from sensor...");
			float temperature = readTemperature();
			if (temperature != -1.0) {
				char tempString[10];
				dtostrf(temperature, 4, 2, tempString);
				UART_SendString("Temperature: ");
				UART_SendString(tempString);
				UART_SendString(" C\n\r");
			}
			break;

			case 'l':
			if (light_toggle == false) {
				UART_SendStringNewLine("Light intensity monitoring started...");
				light_toggle = true;  // Enable periodic light intensity reading
				Timer0_ovf_start_100ms();  // Start Timer0 to trigger every 100ms
				ADC_Init(0x04, 0x02);
				light = ADC_get(3);
				sprintf(out_str, "Light intensity is: %d\r\n", light);
				UART_SendString(out_str);
				} else {
				// Stop periodic light intensity printing
				UART_SendStringNewLine("Light intensity monitoring stopped.");
				light_toggle = false;  // Disable periodic light intensity reading
				Timer0_Stop();  // Stop Timer0
			}
			break;

			default:
			UART_SendStringNewLine("Invalid input, please choose again.");
			break;
		}
	}
}

