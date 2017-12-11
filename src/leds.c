#include <avr/io.h>
#include "leds.h"

void leds_on(void)
{
    PORTC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
}

void leds_off(void)
{
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
}

