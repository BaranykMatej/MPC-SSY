
/*
 * libprintfuart.c
 *
 * Created: 2/24/2025 12:24:51
 *  Author: Student
 */ 
#include "libprintfuart.h"
#include <avr/io.h>
#include <stdio.h>

void UART_init(uint16_t Baudrate) {
	uint16_t ubrr = (F_CPU / 16 / Baudrate) - 1;  // Calculate UBRR value
	UBRR1H = (uint8_t)(ubrr >> 8);  // Set baud rate high byte
	UBRR1L = (uint8_t)ubrr;         // Set baud rate low byte
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);  // Enable RX and TX
}

void UART_SendChar(uint8_t data) {
	while (!(UCSR1A & (1 << UDRE1)));  // Wait for buffer to be empty
	UDR1 = data;  // Send data
}

uint8_t UART_GetChar(void) {
	while (!(UCSR1A & (1 << RXC1)));  // Wait for data to be received
	return UDR1;  // Return received data
}

void UART_SendString(char *text) {
    while (*text != 0x00) {  // Explicitly check for null terminator
        UART_SendChar(*text);
        text++;
    }
}

void UART_SendStringNewLine(char *text) {
    while (*text != 0x00) {  // Explicitly check for null terminator
        UART_SendChar(*text);
        text++;
    }
    UART_SendChar('\r');  // Carriage Return
    UART_SendChar('\n');  // New Line
    UART_SendChar(0x00);  // Explicitly send NULL character
}


int printCHAR(char character, FILE *stream) {
	UART_SendChar(character);  // Send character via UART
	return 0;
}
