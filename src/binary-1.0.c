#include <avr/io.h>
#define F_CPU 3000000UL  // 3 MHz
//#define F_CPU 12000000UL  // 12 MHz
//#define F_CPU 14745600UL  // 14,7456 MHz
#include <util/delay.h>

#define LED_MAX 6
unsigned char leds[LED_MAX] = {PC0, PC1, PC2, PC3, PC4, PC5};

short check_buttons(void)
{
    short buttons, leds;

    buttons = (~PIND & 0x0F);
    leds = PINC;

    while (buttons) 
    {
        PORTC = buttons;
        buttons = (~PIND & 0x0F);
    }
    PORTC = leds;
    return buttons;
}

int pattern(short idx)
{
    short binary = idx;
    while(binary <= 0x3f)
    {
        PORTC = (binary++);
        _delay_ms(50);
    }

    return 0;
}

int main(void)
{
    short i;

    /* enable PC0 - PC3 as output */
    DDRC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    /* enable PD0 - PD3 as input */
    DDRD &= ~(1 << PD0 | 1 << PD1 | 1 << PD2 | 1 << PD3);

    /* turn LEDs off */
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3 | 1 << PC4 | 1 << PC5);
    
    while(1)
    {
        for(i = 0; i < LED_MAX; i++)
        {
            check_buttons();
            /* turn LEDs on */
            PORTC |= (1 << leds[i]);
            _delay_ms(100);
        }
        for(i = (LED_MAX - 1); i >= 0; i--)
        {
            check_buttons();
            /* turn LEDs off again */
            PORTC &= ~(1 << leds[i]);
            _delay_ms(100);
        }
        pattern(0);
        for(i = (LED_MAX - 1); i >= 0; i--)
        {
            check_buttons();
            /* turn LEDs off */
            PORTC &= ~(1 << leds[i]);
            _delay_ms(100);
        }
    }

    goto exit;

exit:
    return 0;
}

