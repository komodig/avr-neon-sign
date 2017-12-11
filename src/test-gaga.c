#include <avr/io.h>
#ifndef F_CPU 
#define F_CPU 12000000UL  // 12 MHz
#endif
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "adconv.h"

#define LED_MAX 4 

#define M_KTY84_FACTOR(x) (((double)x - 500) / 3.9 )

typedef struct temp_s {
    double highest;
    double last;
    uint16_t last_raw;
    double lowest;
} temp_t;

static unsigned char leds[LED_MAX] = {PC0, PC1, PC2, PC3};
static temp_t trec;

static void uart_send_str(char *buf)
{
    short i;
    
    for(i = 0; i < LED_MAX; i++)
    {
        /* turn LEDs on */
        PORTC |= (1 << leds[i]);
        _delay_ms(50);
    }

    uart_puts(buf);
    
    for(i = (LED_MAX - 1); i >= 0; i--)
    {
        /* turn LEDs off again */
        PORTC &= ~(1 << leds[i]);
        _delay_ms(50);
    }
}

static void update_temperature(temp_t *t)
{
    double aval;
    char buffer[50];
    
    t->last_raw = adc_read_channel(4);
    aval = M_KTY84_FACTOR(t->last_raw);

    /* if(aval != t->last) */
    {
        if(t->lowest == 0.0)
            t->lowest = aval;

        t->highest = (aval > t->highest) ? aval : t->highest;
        t->lowest = (aval < t->lowest) ? aval : t->lowest;
        t->last = aval;
    
        sprintf(buffer, "T: %.1lf [%d] low: %.1lf | high: %.1lf\r\n", 
                t->last, t->last_raw, t->lowest, t->highest);
        uart_send_str(buffer);
    }
}

static void main_init(void)
{
    memset(&trec, 0, sizeof(temp_t));
}

int main(void)
{
    main_init();
    uart_init();

    /* enable PC0 - PC3 as output */
    DDRC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);
    
    /* turn LEDs off */
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);
    
    uart_putc(0x0d);
    uart_putc(0x0a);
    uart_puts("avr ready");

    while(1)
    {
    PORTC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);
    _delay_ms(500);
    PORTC |= (1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);
    _delay_ms(500);
    uart_putc(0x0d);
    uart_putc(0x0a);
    uart_puts("tuut");
    }

    while(1)
    {
        update_temperature(&trec);
        _delay_ms(3000);
    }

    return 0;
}
