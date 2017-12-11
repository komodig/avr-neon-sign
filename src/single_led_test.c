#include <avr/io.h>
#include <stdint.h>

#define F_CPU 12000000UL

#include <util/delay.h>

#define PINCOUNT 10 

typedef struct {
    uint8_t pin;
    volatile uint8_t *port;
    uint8_t state;
    volatile uint8_t *dir_reg;
} pinconf_t;


static pinconf_t outpins[PINCOUNT];


void set_pin(pinconf_t *gpio)
{
    *gpio->port |= (1 << gpio->pin); /* set single LED */
    gpio->state = 1;
}


void reset_pin(pinconf_t *gpio)
{
    *gpio->port &= ~(1 << gpio->pin);
    gpio->state = 0;
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


void init_output(pinconf_t *outpin, 
        uint8_t gpio,
        volatile uint8_t *port,
        uint8_t is_set,
        volatile uint8_t *ddreg)
{
    outpin->pin = gpio;
    outpin->port = port;
    outpin->state = is_set;
    outpin->dir_reg = ddreg;

    *ddreg |= (1 << gpio); /* configure gpio as output */    
}


int main(void)
{
    uint8_t x;

    test_output();

    init_output(&outpins[0], PB3, &PORTB, 0, &DDRB);
    init_output(&outpins[1], PB2, &PORTB, 0, &DDRB);
    init_output(&outpins[2], PB1, &PORTB, 0, &DDRB);
    init_output(&outpins[3], PD7, &PORTD, 0, &DDRD);
    init_output(&outpins[4], PD6, &PORTD, 0, &DDRD);
    init_output(&outpins[5], PD4, &PORTD, 0, &DDRD);
    init_output(&outpins[6], PD3, &PORTD, 0, &DDRD);
    init_output(&outpins[7], PD2, &PORTD, 0, &DDRD);
    init_output(&outpins[8], PD5, &PORTD, 0, &DDRD);
    init_output(&outpins[9], PB0, &PORTB, 0, &DDRB);

    while(1)
    {
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
        _delay_ms(1000);
    }

    return 0;
}

