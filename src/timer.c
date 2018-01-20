#include <avr/io.h>
#include <avr/interrupt.h>
#include "led_circle.h"
#include "timer.h"
#include "led_circle.h"


/* signal handler for timer interrupt TOV0 */
ISR(TIMER0_OVF_vect)
{
    ISR_OVF_led_circle();
}

ISR(TIMER0_COMPA_vect)
{
    ISR_CMP_led_circle();
}

void timer_init(void)
{
    /*enable timer overflow interrupt and timer compare interrupt*/
    TIMSK0 |= (1 << TOIE0 | 1 << OCIE0A);
    /* set compare value */
    OCR0A = 64;
    /*set timer counter initial value*/
    TCNT0=0x00;

    sei();
}

void timer_start(uint16_t prescaler)
{
    switch(prescaler)
    {
        case 0:
            TCCR0B = 0x00;
            break;
        default:
        case 1:
        case 8:
            TCCR0B |= (1 << CS01);
            break;
        case 2:
        case 64:
            TCCR0B |= (1 << CS01 | 1 << CS00);
            break;
        case 3:
        case 256:
            TCCR0B |= (1 << CS02);
            break;
        case 4:
        case 1024:
            TCCR0B |= (1 << CS02 | 1 << CS00);
            break;
    }
}

void timer_stop(void)
{
    TCCR0B &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
    TCNT0=0x00;
}

void timer_restart(uint16_t prescaler)
{
    timer_stop();
    timer_start(prescaler);
}

