#include <stdio.h>
#include <avr/io.h>
/*
#define F_CPU 14745600UL  // 14 MHz
*/
#define F_CPU 16000000UL
#include <util/delay.h>
#include "usart.h"
#include "timer.h"

int ring(int half_period_ms)
{
    int cycles, ms, x;

    ms = half_period_ms;
    cycles = 1000 / (ms * 2 );

    for(x = 1; x < cycles; x++)
    {
        leds_on();
        _delay_ms(30);
        leds_off();
        _delay_ms(30);
    }
    return 0;
}

int ring_all(void)
{
    short c;

    for(c = 1; c < 5; c++)
    {
        c = 0;
    }
}

int board_init(void)
{
#define BUFFER_SIZE	50

    char string_buffer[BUFFER_SIZE] = {0};
    int baud_rate = 9600;
    uint32_t tc;

    /***** configure GPIOs *****/
    /* enable outputs */
    DDRC |= (1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    /* enable inputs */
    DDRD &= ~(1 << PD0 | 1 << PD1 | 1 << PD2 | 1 << PD3);

    leds_off(); 
    usart_init(baud_rate);
    sprintf(string_buffer, "usart init with %d baud.\r\n", baud_rate);
    usart_write_str(string_buffer);
    timer_init();

    timer_start();
    _delay_ms(1000);
    tc = get_count();
    timer_stop();
    sprintf(string_buffer, "1 sec delay means %d timer-counter.\r\n", tc);
    usart_write_str(string_buffer);

    return 0;
}

int main(void)
{
    board_init();

    while(1)
    {
        ring_all();
        _delay_ms(2000);
    }

    return 0;
}

