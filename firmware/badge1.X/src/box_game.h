#include <stdint.h>

//Prototypes
void BOX_seed_random(unsigned char);
void BOX_inc_random(void);
uint8_t BOX_get_score(void);
uint16_t  BOX_get_delay(void);
void BOX_clearscreen(void);
void BOX_draw(uint8_t X, uint8_t Y, uint32_t  color);
void BOX_erase(uint8_t X, uint8_t Y);
void BOX_pregame(void);
void BOX_start_game(void);
void BOX_show_gameover(void);
uint8_t BOX_end_game(void);
void BOX_update_score(void);
void BOX_print_string(const int8_t * buf, uint16_t  x_pixel, uint8_t y_pixel, uint32_t  fgcolor, uint32_t  bgcolor);
uint8_t BOX_loc_return_bit(uint8_t X, uint8_t Y);
void BOX_loc_set_bit(uint8_t X, uint8_t Y);
void BOX_loc_clear_bit(uint8_t X, uint8_t Y);
void BOX_store_loc(void);
void BOX_clear_loc(void);
void BOX_load_reference(uint8_t piece, uint8_t rotation);
void BOX_rotate(uint8_t direction);
void BOX_write_piece(void);
void BOX_clear_piece(void);
void BOX_rewrite_display(uint32_t  fgcolor);
void BOX_update_screen(void);
void BOX_spawn(void);
uint8_t BOX_check(int8_t X_offset, int8_t Y_offset);
void BOX_line_check(void);
void BOX_up(void);
void BOX_dn(void);
void BOX_lt(void);
void BOX_rt(void);
void BOX_drop(void);
