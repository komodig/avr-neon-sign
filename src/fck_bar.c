#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define F_CPU 12000000UL

#include <util/delay.h>
#include "gpio.h"
#include "pwm.h"
#include "usart.h"
#include "timer.h"
#include "led_patterns.h"

#define DELAY_MAX 127

#define FCK_BAR 0x44
#define FKK_BAY 0x55

#define I_RED 1
#define C_RED 2
#define I_GREEN 9
#define C_GREEN 10
#define Y_RED 6
#define NOT_Y_RED 7
#define Y_GREEN 14
#define NOT_Y_GREEN 15

static uint8_t level = 0, direction = RISE;


void my_delay(uint16_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
}


void test_soft_pwm(void)
{
    uint8_t x;

    level = 1;
    direction = RISE;
    usart_write_str("test soft pwm\r\n");
    timer_start(PRESCALER);
    reset_all_states(outpins);

    while(1)
    {
        /* set all red pins */
        for(x = 0; x < PINCOUNT/2; ++x)
        {
            set_state(outpins + x);
        }

        if(level <= 0)
        {
            /* direction = RISE; */
            break;
        }
        else if(level >= 0xFF)
        {
            direction = FALL;
        }

        level += (direction == RISE ? 1 : -1);
        timer_stop();
        timer_comparator_set(level);
        timer_start(PRESCALER);
        _delay_us(2000);
    }

    timer_stop();
}


void test_pins(uint8_t pattern)
{
    uint8_t x, y;

    usart_write_str("test pins\r\n");
    x = (pattern == FCK_BAR ? 0 : PINCOUNT/2);
    y = (pattern == FCK_BAR ? PINCOUNT/2 : PINCOUNT);
    for(; x < y; x++)
    {
        if(x == I_RED || x == I_GREEN)
        {
            continue;
        }
        if(x == NOT_Y_RED || x == NOT_Y_GREEN)
        {
            continue;
        }
        usart_write_char(x + 0x30);
        usart_write_char('\r');
        usart_write_char('\n');

        set_pin(outpins + x);

        if(pattern == FCK_BAR && (x == Y_RED || x == Y_GREEN))
        {
            set_pin(outpins + x+1);
        }
        if(pattern == FKK_BAY && (x == C_RED || x == C_GREEN))
        {
            set_pin(outpins + x-1);
        }

        _delay_ms(200);
        reset_pin(outpins + x);

        if(pattern == FCK_BAR && (x == Y_RED || x == Y_GREEN))
        {
            reset_pin(outpins + x+1);
        }
        if(pattern == FKK_BAY && (x == C_RED || x == C_GREEN))
        {
            reset_pin(outpins + x-1);
        }
    }
}


int main(void)
{
    uint8_t x, y;
    init_output(&outpins[1], PD6, &PORTD, &DDRD); // Ir
    init_output(&outpins[2], PD4, &PORTD, &DDRD); // Cr
    init_output(&outpins[3], PC2, &PORTC, &DDRC); // k2r
    init_output(&outpins[4], PC0, &PORTC, &DDRC); // Br
    init_output(&outpins[5], PB1, &PORTB, &DDRB); // Ar
    init_output(&outpins[6], PB4, &PORTB, &DDRB); // Yr
    init_output(&outpins[7], PB3, &PORTB, &DDRB); // notYr

    init_output(&outpins[8], PD7, &PORTD, &DDRD); // Fg
    init_output(&outpins[9], PD5, &PORTD, &DDRD); // Ig
    init_output(&outpins[10], PD3, &PORTD, &DDRD); // Cg
    init_output(&outpins[11], PC3, &PORTC, &DDRC); // k2g
    init_output(&outpins[12], PC1, &PORTC, &DDRC); // Bg
    init_output(&outpins[13], PB2, &PORTB, &DDRB); // Ag
    init_output(&outpins[14], PB5, &PORTB, &DDRB); // Yg
    init_output(&outpins[15], PB0, &PORTB, &DDRB); // notYg

    timer_init();
    usart_init(19200);
    sei();
    usart_write_str("welcome to avr-uno!\r\n");
    test_pins(FCK_BAR);
    test_pins(FKK_BAY);
    test_soft_pwm();

    while(1)
    {
    }

    return 0;
}

