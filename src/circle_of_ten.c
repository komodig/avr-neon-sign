#include <avr/io.h>
#include <stdint.h>

#define F_CPU 12000000UL

#include <util/delay.h>


#define DELAY_MAX 127

#define PINCOUNT 10


typedef struct {
    uint8_t pin;
    volatile uint8_t *port;
    uint8_t state;
    volatile uint8_t *dir_reg;
} pinconf_t;


static pinconf_t outpins[PINCOUNT];

void my_delay(uint8_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
}

uint8_t opposite_of(uint8_t x)
{
    return (x > 4 ? x - 5 : x + 5);
}

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


void set_all(void)
{
    uint8_t x;

    for(x = 0; x < PINCOUNT; x++)
    {
        set_pin(outpins + x);
    }
}


void reset_all(void)
{
    uint8_t x;

    for(x = 0; x < PINCOUNT; x++)
    {
        reset_pin(outpins + x);
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

    while(1)
    {
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

            for(x = 0; x < PINCOUNT; x++)
            {
                set_pin(outpins + x);
                _delay_ms(66);
                reset_pin(outpins + x);
            }

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
            set_all();
            _delay_ms(300);
            reset_all();
            _delay_ms(300);
            set_all();
            _delay_ms(300);
            reset_all();
            _delay_ms(300);
            set_all();
        }

        _delay_ms(300);

        {
            reset_all();
            for(x = 0; x < PINCOUNT/2; x++)
            {
                drop_line_fill(x);
            }
            for(x = 0; x < PINCOUNT/2; x++)
            {
                drop_line_drain(x);
            }
        }

        _delay_ms(300);
    }

    return 0;
}

