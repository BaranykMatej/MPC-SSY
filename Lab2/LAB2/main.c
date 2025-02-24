/*
 * LAB1.c
 *
 * Created: 02.02.2020 9:01:38
 * Author : Ondra
 */ 

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include "libs/macros.h"
#include <avr/io.h>
#include <util/delay.h>
#include "libs/libprintfuart.h"
#include <stdio.h>

/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

#define CONST 2  
#define ODECET  
#define UPPER_CASE 1
#define NORMAL_CASE 2
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2

// F_CPU definovano primo v projektu!!! Debug->Properties->Toolchain->Symbols

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

int vysledek = 10;
unsigned char uch1 = 255;
unsigned char uch2 = 255;

// musime vytvorit soubor pro STDOUT
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();
void generateField(int case_type);   // Prototype for the function to generate the alphabet field
void printField(int direction);      // Prototype for printing the alphabet field
void clearConsole(void);             // Prototype for clear console function

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

// Globalni pole
char abeceda[26];

// Funkce pro generaci abecedy
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
        // spatna vstupni promenna    
        printf("PROGRAM ERROR\n");
        return;
    }
}

// Funkce pro zmenu velikosti pismen
void capsLetters(int case_type) {
    int i;

    if (case_type == UPPER_CASE) {
        for (i = 0; i < 26; i++) {
            abeceda[i] = toupper(abeceda[i]); // zmen na velka
        }
    } else if (case_type == NORMAL_CASE) {
        for (i = 0; i < 26; i++) {
            abeceda[i] = tolower(abeceda[i]); // zmen na mala
        }
    } else {
        printf("PROGRAM ERROR\n");
        return;
    }
}

// Funkce pro vypis abecedy v pozadovanem smeru
void printField(int direction) {
    int i;
    
    if (direction == DIRECTION_UP) {
        for (i = 0; i < 26; i++) {
            UART_SendChar(abeceda[i]); // Send character using UART
            UART_SendChar(' '); // Print space between characters
        }
    } else if (direction == DIRECTION_DOWN) {
        for (i = 25; i >= 0; i--) {
            UART_SendChar(abeceda[i]); // Send character using UART
            UART_SendChar(' '); // Print space between characters
        }
    } else {
        // spatny smer
        UART_SendStringNewLine("PROGRAM ERROR\n");
    }
}

// Function to clear the console (screen)
void clearConsole(void) {
    // Send the ANSI escape code for clearing the screen and resetting the cursor to the top-left
    UART_SendStringNewLine("\033[2J\033[H");
}

void board_init() {
    UART_init(38400); // nastaveni rychlosti UARTu, 38400b/s
    stdout = &uart_str; // presmerovani STDOUT
}

int main(void) {
    UART_init(38400);  // Initialize UART with 38400 baud

    uint8_t test_sequence[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'U', 'A', 'R', 'T', '\r', '\n', 0 };

    for (uint8_t i = 0; test_sequence[i] != 0; i++) {
        UART_SendChar(test_sequence[i]);  // Send each character
    }
    
    UART_SendStringNewLine("Welcome to interactive terminal!");
    UART_SendStringNewLine("Choose a number for response:");
    UART_SendStringNewLine("1 ...... Print lowercase alphabet");
    UART_SendStringNewLine("2 ...... Clear console");
    UART_SendStringNewLine("0 ...... Exit");

    while (1) {
        uint8_t received = UART_GetChar();  // Wait for input
        UART_SendChar(received);  // Echo back received character
        
        if (received == '1') {
            // Generate lowercase alphabet
            generateField(NORMAL_CASE);
            // Print it in the upward direction
            UART_SendStringNewLine("Lowercase Alphabet:");
            printField(DIRECTION_UP);
            UART_SendStringNewLine("");  // New line after printing the alphabet
        } else if (received == '2') {
            // Clear the console
            clearConsole();
        } else if (received == '0') {
            // Exit the loop
            break;
        } else {
            UART_SendStringNewLine("Invalid option. Try again.");
        }
    }

    return 0;
}
