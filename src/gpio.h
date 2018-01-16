#ifndef _GPIO_H_
#define _GPIO_H_

typedef struct {
    uint8_t pin;
    volatile uint8_t *port;
    uint8_t state;
    volatile uint8_t *dir_reg;
} pinconf_t;

extern void init_output(pinconf_t *outpin,
                        uint8_t gpio,
                        volatile uint8_t *port,
                        volatile uint8_t *ddreg);
extern void set_pin(pinconf_t *gpio);
extern void reset_pin(pinconf_t *gpio);
#endif // _GPIO_H_
