#include <avr/io.h>
#include "gpio.h"

void init_output(pinconf_t *outpin,
        uint8_t gpio,
        volatile uint8_t *port,
        volatile uint8_t *ddreg)
{
    outpin->pin = gpio;
    outpin->port = port;
    outpin->state = 0;
    outpin->dir_reg = ddreg;

    *ddreg |= (1 << gpio); /* configure gpio as output */
}

void set_pin(pinconf_t *gpio)
{
    *gpio->port |= (1 << gpio->pin);
}

void reset_pin(pinconf_t *gpio)
{
    *gpio->port &= ~(1 << gpio->pin);
}
