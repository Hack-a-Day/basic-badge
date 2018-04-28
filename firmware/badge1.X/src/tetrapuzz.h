#define s_start 0
#define s_run 1
#define s_freeze 2
#define s_gameover 3

#define DROPRATE  19

void tetrapuzz(unsigned int action);
void tetrapuzz_init(void);
void tetrapuzz_loop(void);
void tetrapuzz_pause(void);
void tetrapuzz_gameover(void);