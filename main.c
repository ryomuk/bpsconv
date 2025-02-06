/* 
 * File:   main.c
 * Author: ryom
 *
 * Created on 2023/02/08, 15:53
 */

// PIC16F18313 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with 2x PLL (32MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled )
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = OFF      // Debugger enable bit (Background debugger disabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 32000000

#define BPS_IN 9600
#define DELAY_IN (1000000/BPS_IN)
#define BPS_OUT 38400

unsigned char myGetchar() {
  int i;
  unsigned char c = 0;

  while(PORTAbits.RA1){} /* wait for start bit */
  __delay_us(DELAY_IN*3/2);

  for(i = 0; i < 8; i++){
    c = c | ((unsigned char) PORTAbits.RA1 << i);
    __delay_us(DELAY_IN);
  }

  return(c);
}

void myPutchar(unsigned char c) {
  while(!TXIF){}
  TX1REG = c;

    __delay_us(100);
}


/*
 * 
 */
int main(int argc, char** argv) {
    int i;

    ANSELA = 0x00;
    TRISA = 0b111010;

    WPUAbits.WPUA1 = 1;
    
    RA2PPS = 0b10100; // RA2 is TX

    RC1STA   = 0b10000000;
    TX1STA   = 0b00100111; 
    BAUD1CON = 0b00001000;
    SP1BRGH = 1;
    SP1BRG = 207; // 38400bps
    
    for(i = 0; i < 3; i++){
        PORTAbits.RA0=1;
        __delay_ms(150);
        PORTAbits.RA0=0;
        __delay_ms(150);
    }

    while(1){
      myPutchar(myGetchar());
      PORTAbits.RA0 = ~PORTAbits.RA0;
    }
    return (EXIT_SUCCESS);
}

