void TFT_24_7789_Init(void);
inline void TFT_24_7789_Write_Data(uint16_t data1);
inline void TFT_24_7789_Write_Data3(uint16_t data1,uint16_t data2, uint16_t data3);
void TFT_24_7789_Write_Command(uint16_t command);
inline void tft_set_write_area (uint16_t x, uint16_t y, uint16_t xlen, uint16_t ylen);
inline void tft_print_char (uint8_t val, uint16_t x, uint16_t y, uint16_t col, uint16_t back);
void tft_fill_area (uint16_t x, uint16_t y, uint16_t xlen, uint16_t ylen, uint16_t back);
void tft_disp_buffer_refresh(uint8_t * buff, uint8_t * color_buff);
void tft_disp_buffer_refresh_part(uint8_t * buff, uint8_t * color_buff);


