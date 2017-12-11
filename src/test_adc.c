#include <avr/io.h>
#ifndef F_CPU 
#define F_CPU 12000000UL  // 12 MHz
#endif
#include <util/delay.h>
#include "uart.h"
#include "adconv.h"

#define LED_MAX 4 

unsigned char leds[LED_MAX] = {PC0, PC1, PC2, PC3};

int main(void)
{
    short i;
    uint16_t aval;
    char buffer[20]; 

    uart_init();

    /* enable PC0 - PC3 as output */
    DDRC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);
    
    /* turn LEDs off */
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);
    
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
        aval = adc_read_channel(4);
        itoa(aval, buffer, 10 );
        uart_puts(buffer);
        
        for(i = (LED_MAX - 1); i >= 0; i--)
        {
            /* turn LEDs off again */
            PORTC &= ~(1 << leds[i]);
            _delay_ms(50);
        }
        
        _delay_ms(1000);
    }

    return 0;
}
