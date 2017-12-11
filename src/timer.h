#ifndef _TIMER_H_
#define _TIMER_H_
extern void timer_init(void);
extern void timer_start(void);
extern void timer_stop(void);
extern void timer_restart(void);
extern uint32_t get_count(void);
#endif // _TIMER_H_
