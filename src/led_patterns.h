#ifndef _LED_PATTERNS_H_
#define _LED_PATTERNS_H_
#include "gpio.h"

#define PINCOUNT 16
#define PRESCALER 2
#define RISE 1
#define FALL 0

extern pinconf_t outpins[PINCOUNT];

extern void ISR_OVF_led_patterns(void);
extern void ISR_CMP_led_patterns(void);
extern void set_state(pinconf_t *pinobj);
extern void set_all(pinconf_t *outpins);
extern void reset_all(pinconf_t *outpins);
extern void reset_all_states(pinconf_t *outpins);
extern uint8_t opposite_of(uint8_t x);
extern void increase_circular_ptr(pinconf_t **led_to_set, pinconf_t *outpins);
extern void set_range(uint8_t start, uint8_t count, pinconf_t *outpins);
extern void reset_range(uint8_t start, uint8_t count, pinconf_t *outpins);
#endif /* ifndef _LED_PATTERNS_H_ */
