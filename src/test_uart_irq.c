#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU 
#define F_CPU 12000000UL  // 12 MHz
#endif
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

#define LED_MAX 6
#define BUFFER_SIZE 10

static unsigned char buf[BUFFER_SIZE + 1] = {0}; 

int main(void)
{
    unsigned char last_bcount = 0;
    unsigned char *pbuf = buf;
    char itos_buf[16 + 1] = {0};

    uart_init();
    sei();  /* enable interrupts */
    /* enable PC0 - PC3 as output */
    DDRC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    /* turn LEDs on/off */
    PORTC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    _delay_ms(1000);
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    uart_putc(0x0d);
    uart_putc(0x0a);
    uart_puts("avr ready");

    while(1)
    {
        if(uart_get_rxcnt() > last_bcount)
        {
            last_bcount = uart_get_rxcnt();
            PORTC = (unsigned char)last_bcount;  
            *pbuf = uart_getc();
            ++pbuf;
        }
        if((uart_get_rxcnt() % BUFFER_SIZE) == 0)
        {
            _delay_ms(500);
            uart_putc(0x0d);
            uart_putc(0x0a);
            uart_puts(buf);
            uart_putc('(');
            sprintf(itos_buf, "%d", last_bcount);
            uart_putc(')');
            pbuf = buf;
        }
    }

    return 0;
}
