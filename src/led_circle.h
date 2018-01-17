#ifndef _LED_CIRCLE_H_
#define _LED_CIRCLE_H_
#include "gpio.h"

#define PINCOUNT 10
#define RISE 1
#define FALL 0

extern pinconf_t outpins[PINCOUNT];

extern void isr_led_circle(void);
extern void set_all(pinconf_t *outpins);
extern void reset_all(pinconf_t *outpins);
extern uint8_t opposite_of(uint8_t x);
extern void increase_circular_ptr(pinconf_t **led_to_set, pinconf_t *outpins);
extern void set_range(uint8_t start, uint8_t count, pinconf_t *outpins);
extern void reset_range(uint8_t start, uint8_t count, pinconf_t *outpins);
#endif /* ifndef _LED_CIRCLE_H_ */
