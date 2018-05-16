#include <stdint.h>

uint8_t add_prog_line (int8_t * line, int8_t * prog, int16_t linenum);
void terminal_init(void);
int8_t term_k_stat (void);
int8_t term_k_char (int8_t * out);
uint8_t rx_sta (void);
uint8_t rx_read (void);
void tx_write (uint8_t data);
uint8_t cmd_exec (int8_t * cmd);
uint8_t basic_save_program (uint8_t * data, uint8_t slot);
uint8_t basic_load_program (uint8_t * data, uint8_t slot);
uint16_t get_free_mem(uint8_t * prog, uint16_t max_mem);
const char* get_firmware_string(void);
uint16_t basic_loads (int8_t * data, uint16_t maxlen);
uint16_t basic_saves (int8_t * data, uint16_t maxlen);
void init_z80_cpm (void);
void init_basic (void);
void init_userprog(void);
void loop_z80_cpm (void);
void loop_basic (void);
void loop_userprog(void);
void boot_animation(void);
void init_8080_basic (void);
void loop_8080_basic (void);
void clr_buffer(void);
void loop_badge(void);
void enable_display_scanning(uint8_t onoff);
uint32_t millis(void);
void list_more (void);

uint8_t stdio_local_buffer_state (void);
int8_t stdio_local_buffer_get (void);
void stdio_local_buffer_put (int8_t data);
void stdio_local_buffer_puts (int8_t * data);

void menu(void);
uint32_t hash(int8_t *);
uint8_t get_command_index(uint32_t );
uint8_t wisecrack(int8_t *, uint16_t , unsigned char);
void showmenu(void);
void fancyframe(void);
void clear_prompt(void);
void show_wrencher(void);
uint8_t playriff(unsigned char);


#define STDIO_LOCAL_BUFF_SIZE	25

//Prompt handling defines
#define COMMAND_MAX 32
#define TEXT_LEFT	4
#define PROMPT_Y	15
#define CRACK_Y		17
#define VERSION_X	33
#define VERSION_Y	18

//Menu color values
#define MENU_FRAME_FG	12
#define MENU_FRAME_BG	0
#define MENU_BANNER_FG	0
#define MENU_BANNER_BG	15
#define MENU_HEADER_FG	15
#define MENU_HEADER_BG	8
#define MENU_ENTRY_FG	15
#define MENU_ENTRY_BG	9
#define MENU_DEFAULT_FG 15
#define MENU_DEFAULT_BG 0
#define MENU_VERSION_FG	8


