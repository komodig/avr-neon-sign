#include <avr/io.h>
#include "led_patterns.h"
#include "gpio.h"


pinconf_t outpins[PINCOUNT];


/*
 * pin manipulation directly
 */
uint8_t opposite_of(uint8_t x)
{
    return (x > 4 ? x - 5 : x + 5);
}


void set_all(pinconf_t *outpins)
{
    uint8_t x;

    for(x = 0; x < PINCOUNT; x++)
    {
        set_pin(outpins + x);
    }
}


void reset_all(pinconf_t *outpins)
{
    uint8_t x;

    for(x = 0; x < PINCOUNT; x++)
    {
        reset_pin(outpins + x);
    }
}


void increase_circular_ptr(pinconf_t **led_to_set, pinconf_t *outpins)
{
    if(*led_to_set < (outpins + PINCOUNT))
        (*led_to_set)++;
    else
        *led_to_set = outpins;
}


void set_range(uint8_t start, uint8_t count, pinconf_t *outpins)
{
    uint8_t x;
    pinconf_t *led_to_set = outpins + start;

    for(x = 0; x < count; x++)
    {
        set_pin(led_to_set);
        increase_circular_ptr(&led_to_set, outpins);
    }
}


void reset_range(uint8_t start, uint8_t count, pinconf_t *outpins)
{
    uint8_t x;
    pinconf_t *led_to_set = outpins + start;

    for(x = 0; x < count; x++)
    {
        reset_pin(led_to_set);
        increase_circular_ptr(&led_to_set, outpins);
    }
}


/*
 * pinconf object states to indirectly set pins
 */
void all_state_to_pins(pinconf_t *outpins)
{
    uint8_t x;

    for(x = 0; x < PINCOUNT; x++)
    {
        if((outpins + x)->state == 1)
        {
            set_pin(outpins + x);
        }
        else
        {
            reset_pin(outpins + x);
        }
    }
}


void set_state(pinconf_t *pinobj)
{
    pinobj->state = 1;
}


void reset_all_states(pinconf_t *outpins)
{
    uint8_t x;

    for(x = 0; x < PINCOUNT; x++)
    {
        (outpins + x)->state = 0;
    }
}


/* 
 * interrupt service routines
 */
void ISR_OVF_led_patterns(void)
{
    timer_restart(PRESCALER);
    all_state_to_pins(outpins);
}


void ISR_CMP_led_patterns(void)
{
    reset_all(outpins);
}


