void TFT_24_7789_Init(void);
inline void TFT_24_7789_Write_Data(unsigned int data1);
inline void TFT_24_7789_Write_Data3(unsigned int data1,unsigned int data2, unsigned int data3);
void TFT_24_7789_Write_Command(unsigned int command);
inline void tft_set_write_area (unsigned int x, unsigned int y, unsigned int xlen, unsigned int ylen);
inline void tft_print_char (unsigned char val, unsigned int x, unsigned int y, unsigned int col, unsigned int back);
void tft_fill_area (unsigned int x, unsigned int y, unsigned int xlen, unsigned int ylen, unsigned int back);
void tft_disp_buffer_refresh(unsigned char * buff, unsigned char * color_buff);
void tft_disp_buffer_refresh_part(unsigned char * buff, unsigned char * color_buff);


