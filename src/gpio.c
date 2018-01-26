#include <avr/io.h>
#include "gpio.h"
#include "usart.h"

void init_output(pinconf_t *outpin,
        uint8_t gpio,
        volatile uint8_t *port,
        volatile uint8_t *ddreg,
        uint8_t *letters)
{
    outpin->pin = gpio;
    outpin->port = port;
    outpin->state = 0;
    outpin->dir_reg = ddreg;
    outpin->letters = letters;

    *ddreg |= (1 << gpio); /* configure gpio as output */
}

inline void set_pin(pinconf_t *gpio)
{
    *gpio->port |= (1 << gpio->pin);
}

inline void reset_pin(pinconf_t *gpio)
{
    *gpio->port &= ~(1 << gpio->pin);
}

inline uint8_t is_letter(pinconf_t *outpin, letters_t letter_needed)
{
    uint8_t *letter = outpin->letters;
    while(*letter != '\0')
    {
        if(letter_needed == *letter)
            return 1;

        ++letter;
    }
    return 0;
}
