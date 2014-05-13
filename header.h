#include <stdint.h>

extern void display_init();
extern void display(uint16_t n);

extern void cap_init(void);
extern uint16_t freq_query();

extern void gen_init();
extern void gen_start();
extern void gen_set(uint16_t freq);
extern void gen_stop();
