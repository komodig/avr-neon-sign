#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

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


uint8_t opposite_of(uint8_t x)
{
    return (x > 4 ? x - 5 : x + 5);
}


void increase_circular_ptr(pinconf_t **led_to_set)
{
    if(*led_to_set < (outpins + PINCOUNT))
        (*led_to_set)++;
    else
        *led_to_set = outpins;
}


void set_range(uint8_t start, uint8_t count)
{
    uint8_t x;
    pinconf_t *led_to_set = outpins + start;

    for(x = 0; x < count; x++)
    {
        set_pin(led_to_set);
        increase_circular_ptr(&led_to_set);
    }
}


void reset_range(uint8_t start, uint8_t count)
{
    uint8_t x;
    pinconf_t *led_to_set = outpins + start;

    for(x = 0; x < count; x++)
    {
        reset_pin(led_to_set);
        increase_circular_ptr(&led_to_set);
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


/* Here is an example code of 8-bit Timer2 Fast PWM Mode (8KHz) at 16MHz clock */
void config_pwm(uint16_t ocra2_val)
{
    /* PB3 as output */
    DDRB |= (1 << PB3);
    /* e.g. set PWM for 50% duty cycle by ocra2_val = 128 */
    OCR2A = ocra2_val & 0xFF;
    OCR2B = (ocra2_val >> 8) & 0xFF;
     /* set non-inverting mode */
    TCCR2A |= (1 << COM2A1);
    /* set fast PWM Mode */
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    /* set prescaler to 8 and starts PWM */
    TCCR2B |= (1 << CS21);
}


void disable_pwm(void)
{
        TCCR2A = 0;
        TCCR2B = 0;
        OCR2A = 0;
        OCR2B = 0;
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
        /* program 1 */
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
        }

        /* program 2 */
        {
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

        /* program 3 */
        {
            for(y = 1; y < PINCOUNT; y++)
            {
                for(x = 0; x < PINCOUNT; x++)
                {
                    reset_range(x, y);
                    _delay_ms(33);
                    set_range(x, y);
                }
                if(y < 5)
                {
                    for(x = PINCOUNT; x > 0; x--)
                    {
                        reset_range(x, y);
                        _delay_ms(33);
                        set_range(x, y);
                    }
                }
                else
                {
                    for(x = 0; x < PINCOUNT; x++)
                    {
                        reset_range(x, y);
                        _delay_ms(33);
                        set_range(x, y);
                    }
                }
            }
            reset_all();
            _delay_ms(300);
            set_all();
            _delay_ms(300);
            reset_all();
        }

        _delay_ms(300);

        /* program 4 */
        {
            for(x = 0 ; x < 64; x++)
            {
                srand(x);
                y = rand() % 10;
                set_pin(outpins + y);
                _delay_ms(44);
                if(x % 4 == 0)
                {
                    reset_all();
                }
            }
        }
        /* program 5 */
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

        /* test PWM */
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

    return 0;
}

