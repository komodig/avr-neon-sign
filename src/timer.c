#include <avr/interrupt.h>
#include "timer.h"
#include "leds.h"

static uint32_t counter;

/* signal handler for timer interrupt TOV0 */
ISR(TIMER0_OVF_vect)
{
    counter++;
    if(counter > 1125)  /* ca. 5 seconds */
    {
        leds_on();
    }
}

void timer_init(void)
{
    /*enable timer overflow interrupt*/
    TIMSK |= (1 << TOV0);
    /*set timer counter initial value*/
    TCNT0=0x00;
}

void timer_start(void)
{
    counter = 0;
    /* start timer with prescaler 256 (225 Overflows/second)*/
    TCCR0 |= (1 << CS02);
}

void timer_stop(void)
{
    TCCR0 &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
    TCNT0=0x00;
}

void timer_restart(void)
{
    timer_stop();
    timer_start();
}

uint32_t get_count(void)
{
    return counter;
}

