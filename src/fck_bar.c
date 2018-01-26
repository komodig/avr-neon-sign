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

static const uint8_t FCK_BAR[LETTERCOUNT] = { F_RED, C_RED, K2_RED, B_RED, A_RED, R_RED };
static const uint8_t FKK_BAY[LETTERCOUNT] = { F_GREEN, K_GREEN, K2_GREEN, B_GREEN, A_GREEN, Y_GREEN };

/* to initialize which pinconf is needed for what letter */
static const uint8_t pin_for_letters[PINCOUNT][3] =
{
    { F_RED, '\0' },
    { K_RED, '\0' },
    { C_RED, K_RED, '\0' },
    { K2_RED, '\0' },
    { B_RED, '\0' },
    { A_RED, '\0' },
    { Y_RED, R_RED, '\0' },
    { R_RED, '\0' },
    { F_GREEN, '\0' },
    { K_GREEN, '\0' },
     { C_GREEN, K_GREEN, '\0' },
     { K2_GREEN, '\0' },
     { B_GREEN, '\0' },
     { A_GREEN, '\0' },
     { Y_GREEN, R_GREEN, '\0' },
     { R_GREEN, '\0' }
};

static uint8_t level = 0, direction = RISE;


void my_delay(uint16_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
}


void test_soft_pwm(uint16_t on_time)
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
            if(x == 1)
            {
                continue;
            }
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
            my_delay(on_time);
        }

        level += (direction == RISE ? 1 : -1);
        timer_stop();
        timer_comparator_set(level);
        timer_start(PRESCALER);
        _delay_us(2000);
    }

    timer_stop();
}


void test_pins(uint8_t *pattern)
{
    uint8_t x;

    usart_write_str("test pins\r\n");
    for(x = 0; x < LETTERCOUNT; x++)
    {
        set_letter(pattern + x, outpins);
        _delay_ms(200);
        reset_letter(pattern + x, outpins);
    }
}


void test_mixed_pins(uint8_t *pattern)
{
    uint8_t x;
    const uint8_t PAT2[LETTERCOUNT] = { F_GREEN, K_GREEN, K2_GREEN, B_GREEN, A_RED, Y_GREEN };

    usart_write_str("test mixed pins\r\n");
    for(x = 0; x < LETTERCOUNT; x++)
    {
        set_letter(pattern + x, outpins);
    }
    _delay_ms(1500);
    reset_letter(pattern + 4, outpins);
    set_letter((uint8_t *)&PAT2[4], outpins);
    _delay_ms(200);
    reset_letter((uint8_t *)&PAT2[4], outpins);
    _delay_ms(100);
    reset_letter(pattern + 4, outpins);
    set_letter((uint8_t *)&PAT2[4], outpins);
    _delay_ms(300);
    reset_all(outpins);
    set_letter((uint8_t *)&PAT2[4], outpins);
    _delay_ms(400);
}


int main(void)
{
    uint8_t x, y;

    init_output(&outpins[0], PD2, &PORTD, &DDRD, (uint8_t *)&pin_for_letters[0]); // Fr
    init_output(&outpins[1], PD6, &PORTD, &DDRD, (uint8_t *)&pin_for_letters[1]); // Ir
    init_output(&outpins[2], PD4, &PORTD, &DDRD, (uint8_t *)&pin_for_letters[2]); // Cr
    init_output(&outpins[3], PC2, &PORTC, &DDRC, (uint8_t *)&pin_for_letters[3]); // k2r
    init_output(&outpins[4], PC0, &PORTC, &DDRC, (uint8_t *)&pin_for_letters[4]); // Br
    init_output(&outpins[5], PB1, &PORTB, &DDRB, (uint8_t *)&pin_for_letters[5]); // Ar
    init_output(&outpins[6], PB4, &PORTB, &DDRB, (uint8_t *)&pin_for_letters[6]); // Yr
    init_output(&outpins[7], PB3, &PORTB, &DDRB, (uint8_t *)&pin_for_letters[7]); // Rr
    init_output(&outpins[8], PD7, &PORTD, &DDRD, (uint8_t *)&pin_for_letters[8]); // Fg
    init_output(&outpins[9], PD5, &PORTD, &DDRD, (uint8_t *)&pin_for_letters[9]); // Ig
    init_output(&outpins[10], PD3, &PORTD, &DDRD, (uint8_t *)&pin_for_letters[10]); // Cg
    init_output(&outpins[11], PC3, &PORTC, &DDRC, (uint8_t *)&pin_for_letters[11]); // k2g
    init_output(&outpins[12], PC1, &PORTC, &DDRC, (uint8_t *)&pin_for_letters[12]); // Bg
    init_output(&outpins[13], PB2, &PORTB, &DDRB, (uint8_t *)&pin_for_letters[13]); // Ag
    init_output(&outpins[14], PB5, &PORTB, &DDRB, (uint8_t *)&pin_for_letters[14]); // Yg
    init_output(&outpins[15], PB0, &PORTB, &DDRB, (uint8_t *)&pin_for_letters[15]); // Rg

    timer_init();
    usart_init(19200);
    sei();
    usart_write_str("welcome to avr-uno!\r\n");

    while(1)
    {
        test_pins((uint8_t *)FCK_BAR);
        test_soft_pwm(2000);
        _delay_ms(500);
        test_soft_pwm(3000);
        _delay_ms(500);
        test_pins((uint8_t *)FKK_BAY);
        test_mixed_pins((uint8_t *)FKK_BAY);
        _delay_ms(1000);
        reset_all(outpins);
        _delay_ms(1000);
    }

    return 0;
}

