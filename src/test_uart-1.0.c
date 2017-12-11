/*********************************************
* vim: set sw=8 ts=8 si :
* Author: Guido Socher, Copyright: GPL 
* This program is to test the led connected to
* PC5. 
* See http://linuxfocus.org/English/November2004/
* for details.
* Chip type           : ATMEGA8
* Clock frequency     : Internal clock 1 Mhz (factory default)
*********************************************/
#include <avr/io.h>
#ifndef F_CPU 
#define F_CPU 12000000UL  // 12 MHz
#endif
#include <util/delay.h>
#include "uart.h"

#define LED_MAX 6

unsigned char leds[LED_MAX] = {PC0, PC1, PC2, PC3, PC4, PC5};

int main(void)
{
    short i;

    uart_init();

    /* enable PC0 - PC3 as output */
    DDRC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    /* turn LEDs off */
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    uart_putc(0x0d);
    uart_putc(0x0a);
    uart_puts("avr ready");

    while(1)
    {
        for(i = 0; i < LED_MAX; i++)
        {
            /* turn LEDs on */
            PORTC |= (1 << leds[i]);
            _delay_ms(50);
        }

        uart_putc(0x0d);
        uart_putc(0x0a);
        uart_puts("RX:");
        uart_putc(uart_getc());
        
        for(i = (LED_MAX - 1); i >= 0; i--)
        {
            /* turn LEDs off again */
            PORTC &= ~(1 << leds[i]);
            _delay_ms(50);
        }
    }

    return 0;
}
