#ifndef _TIMER_H_
#define _TIMER_H_
extern void timer_init(void);
extern void timer_start(uint16_t prescaler);
extern void timer_stop(void);
extern void timer_restart(uint16_t prescaler);
extern uint32_t get_count(void);
#endif // _TIMER_H_
