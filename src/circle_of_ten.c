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
#include "led_circle.h"

#define DELAY_MAX 127


static uint8_t level = 0, direction = RISE;


void my_delay(uint16_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
}


void drop_line_fill(uint8_t level)
{
    uint16_t time = 133;

    set_pin(outpins + 3);
    set_pin(outpins + 4);
    _delay_ms(time);
    if(level == 4)
        return;
    reset_pin(outpins + 3);
    reset_pin(outpins + 4);
    set_pin(outpins + 2);
    set_pin(outpins + 5);
    _delay_ms(time);
    if(level == 3)
        return;
    reset_pin(outpins + 2);
    reset_pin(outpins + 5);
    set_pin(outpins + 1);
    set_pin(outpins + 6);
    _delay_ms(time);
    if(level == 2)
        return;
    reset_pin(outpins + 1);
    reset_pin(outpins + 6);
    set_pin(outpins + 0);
    set_pin(outpins + 7);
    _delay_ms(time);
    if(level == 1)
        return;
    reset_pin(outpins + 0);
    reset_pin(outpins + 7);
    set_pin(outpins + 8);
    set_pin(outpins + 9);
}


void drop_line_drain(uint8_t level)
{
    uint16_t time = 166;

    switch(level)
    {
        case 4:
            reset_pin(outpins + 3);
            reset_pin(outpins + 4);
            set_pin(outpins + 2);
            set_pin(outpins + 5);
            _delay_ms(time);
        case 3:
            reset_pin(outpins + 2);
            reset_pin(outpins + 5);
            set_pin(outpins + 1);
            set_pin(outpins + 6);
            _delay_ms(time);
        case 2:
            reset_pin(outpins + 1);
            reset_pin(outpins + 6);
            set_pin(outpins + 0);
            set_pin(outpins + 7);
            _delay_ms(time);
        case 1:
            reset_pin(outpins + 0);
            reset_pin(outpins + 7);
            set_pin(outpins + 8);
            set_pin(outpins + 9);
            _delay_ms(time);
        case 0:
        default:
            reset_pin(outpins + 8);
            reset_pin(outpins + 9);
            _delay_ms(time);
    }
}

void test_output(void)
{
    uint8_t x;

    DDRB |= (1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
    DDRD |= (1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);

    for(x = 1; x < 3; x++)
    {
        /* turn LEDs off */
        PORTB &= ~(1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
        PORTD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
        _delay_ms(111);
        /* turn LEDs on */
        PORTB |= (1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
        PORTD |= (1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
        _delay_ms(111);
    }

    /* turn LEDs off */
    PORTB &= ~(1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
    PORTD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
}


void program1(void)
{
    uint8_t x, y;

    for(x = 0; x < PINCOUNT; x++)
    {
        set_pin(outpins + x);
        _delay_ms(66);
    }

    for(x = 0; x < PINCOUNT; x++)
    {
        reset_pin(outpins + x);
        _delay_ms(66);
        set_pin(outpins + x);
    }

    for(x = 0; x < PINCOUNT; x++)
    {
        reset_pin(outpins + x);
        _delay_ms(66);
    }

    for(x = 0; x < PINCOUNT; x++)
    {
        set_pin(outpins + x);
        _delay_ms(66);
        reset_pin(outpins + x);
    }
}


void program2(void)
{
    uint8_t x, y;

    y = 66;
    while(y)
    {
        for(x = 0; x < PINCOUNT; x++)
        {
            set_pin(outpins + x);
            set_pin(outpins + opposite_of(x));
            my_delay(y);
            reset_pin(outpins + x);
            reset_pin(outpins + opposite_of(x));
        }
        y -= 6;
    }
    set_all(outpins);
    _delay_ms(300);
    reset_all(outpins);
    _delay_ms(300);
    set_all(outpins);
    _delay_ms(300);
    reset_all(outpins);
    _delay_ms(300);
    set_all(outpins);
}


void program3(void)
{
    uint8_t x, y;

    for(y = 1; y < PINCOUNT; y++)
    {
        for(x = 0; x < PINCOUNT; x++)
        {
            reset_range(x, y, outpins);
            _delay_ms(33);
            set_range(x, y, outpins);
        }
        if(y < 5)
        {
            for(x = PINCOUNT; x > 0; x--)
            {
                reset_range(x, y, outpins);
                _delay_ms(33);
                set_range(x, y, outpins);
            }
        }
        else
        {
            for(x = 0; x < PINCOUNT; x++)
            {
                reset_range(x, y, outpins);
                _delay_ms(33);
                set_range(x, y, outpins);
            }
        }
    }
    reset_all(outpins);
    _delay_ms(300);
    set_all(outpins);
    _delay_ms(300);
    reset_all(outpins);
}


void program4(void)
{
    uint8_t x, y;

    for(x = 0 ; x < 64; x++)
    {
        srand(x);
        y = rand() % 10;
        set_pin(outpins + y);
        _delay_ms(44);
        if(x % 4 == 0)
        {
            reset_all(outpins);
        }
    }
}


void program5(void)
{
    uint8_t x, y;

    reset_all(outpins);
    for(x = 0; x < PINCOUNT/2; x++)
    {
        drop_line_fill(x);
    }
    for(x = 0; x < PINCOUNT/2; x++)
    {
        drop_line_drain(x);
    }
}


void test_pwm(void)
{
    uint8_t x, y;

    for(y = 0; y < 3; y++)
    {
        for(x = 0; x < 128; x++)
        {
            config_pwm(x);
            _delay_ms(10);
        }
        for(x = 128; x > 0; x--)
        {
            config_pwm(x);
            _delay_ms(10);
        }
    }
    _delay_ms(300);
    disable_pwm();
}


int main(void)
{
    uint8_t x, y;

    test_output();

    init_output(&outpins[0], PB3, &PORTB, &DDRB);
    init_output(&outpins[1], PB2, &PORTB, &DDRB);
    init_output(&outpins[2], PB1, &PORTB, &DDRB);
    init_output(&outpins[3], PD7, &PORTD, &DDRD);
    init_output(&outpins[4], PD6, &PORTD, &DDRD);
    init_output(&outpins[5], PD4, &PORTD, &DDRD);
    init_output(&outpins[6], PD3, &PORTD, &DDRD);
    init_output(&outpins[7], PD2, &PORTD, &DDRD);
    init_output(&outpins[8], PD5, &PORTD, &DDRD);
    init_output(&outpins[9], PB0, &PORTB, &DDRB);

    timer_init();
    usart_init(19200);
    sei();
    timer_start(2);
    usart_write_str("welcome to avr-uno!\r\n");

    while(1)
    {
        if(level >= 0xFF)
        {
            direction = FALL;
        }
        else if(level <= 0)
        {
            direction = RISE;
        }

        level += (direction == RISE ? 1 : -1);

        timer_stop();
        timer_comparator_set(level);
        timer_start(2);
        my_delay(5);
    }

    return 0;
}

