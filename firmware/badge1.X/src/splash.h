/*
 * Graphic from poster located here:
 * https://cdn.hackaday.io/images/5980891518077065617.png
 * 
 * Napkin calculations:
 *  Original spacing is 347px letters with 32px spaces
 *  Combining character with spaces calls for 8.44% space
 *  240px screen, 4 characters
 *  Characters should be 54px (18px subchars each) with 5px spacing and 3px on top
 */

#define CIPHER_X0           42
#define CIPHER_Y0           2
#define CIPHER_CHAR_WIDTH   18
#define CIPHER_SPACE        5
#define CIPHER_COLOR        0xEE3530

void show_splash(void);
void draw_vert_line(unsigned int x, unsigned char y, unsigned char height, unsigned int color);
void animate_splash(void);

const unsigned int b_cipher[12] = {
    0b0000101010111101,
    0b0000010111100110,
    0b0000101101111101,
    
    0b0000010110010101,
    0b0000111101111010,
    0b0000101110101010,
    
    0b0000100111100110,
    0b0000111110100101,
    0b0000111111111111,
    
    0b0000111010110111,
    0b0000110111101110,
    0b0000101101110111
};