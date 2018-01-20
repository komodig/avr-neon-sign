#include <avr/io.h>
#include "led_circle.h"
#include "gpio.h"


pinconf_t outpins[PINCOUNT];
static uint8_t state = 0, direction = RISE, level = 1;
static uint16_t counter = 0;


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


uint8_t opposite_of(uint8_t x)
{
    return (x > 4 ? x - 5 : x + 5);
}


void isr_led_circle(void)
{
    if(counter == 1)
    {
        if(state == 0)
        {
            set_all(outpins);
            state = 1;
            timer_restart(8);
        }
        else
        {
            reset_all(outpins);
            state = 0;
            timer_restart(level);
        }
        if(level > 4)
        {
            level = 0;
        }
        ++level;
        counter = 0;
    }
    ++counter;
    return;
    /*
    if(counter % 1 == 0)
    {
        if(state == 0)
        {
            set_all(outpins);
            state = 1;
            timer_restart(8);
        }
    }
    if(counter % level == 0)
    {
        if(state == 1)
        {
            reset_all(outpins);
            state = 0;

            if(level >= 265)
            {
                direction = FALL;
            }
            if(level <= 0)
            {
                direction = RISE;
            }
            level += (direction == RISE ? 1 : -1);

            timer_restart(8);
        }
    }
    ++counter;
    */
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


