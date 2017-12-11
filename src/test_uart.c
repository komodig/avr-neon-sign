#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU 
#define F_CPU 12000000UL  // 12 MHz
#endif
#include <util/delay.h>
#include "uart.h"

#define LED_MAX 6
#define BUFFER_SIZE 10

static unsigned char buf[BUFFER_SIZE + 1] = {0}; 
static unsigned char bcount = 0;

ISR(USART_RXC_vect)
{
    ++bcount;
}

int main(void)
{
    unsigned char last_bcount = 0;
    unsigned char *pbuf = buf;

    uart_init();
#if 0
    sei();  /* enable interrupts */
#endif
    /* enable PC0 - PC3 as output */
    DDRC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    /* turn LEDs off */
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    uart_putc(0x0d);
    uart_putc(0x0a);
    uart_puts("avr ready");

    while(1)
    {
        if(bcount > last_bcount)
        {
            last_bcount = PORTC = bcount;
            *pbuf = uart_getc();
            ++pbuf;
        }
        if((bcount % BUFFER_SIZE) == 0)
        {
            _delay_ms(500);
            uart_putc(0x0d);
            uart_putc(0x0a);
            uart_puts(buf);
            pbuf = buf;
        }
    }

    return 0;
}
