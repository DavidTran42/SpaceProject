#define ESC 0x1B
#include "ansi.h"
#include "Excellutex.h"
#include "charset.h"
#include "30010_io.h"


void fgcolor(uint8_t foreground) {
	/*  Value      foreground     Value     foreground
	 ------------------------------------------------
	 0        Black            8       Dark Gray
	 1        Red              9       Light Red
	 2        Green           10       Light Green
	 3        Brown           11       Yellow
	 4        Blue            12       Light Blue
	 5        Purple          13       Light Purple
	 6        Cyan            14       Light Cyan
	 7        Light Gray      15       White
	 */
	uint8_t type = 22;             // normal text
	if (foreground > 7) {
		type = 1;                // bold text
		foreground -= 8;
	}
	printf("%c[%d;%dm", ESC, type, foreground + 30);
}

void bgcolor(uint8_t background) {
	/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
	 Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
	 The designers of HyperTerminal, however, preferred black text on white background, which is why
	 the colors are initially like that, but when the background color is first changed there is no
	 way comming back.

	 Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

	 Value      Color
	 ------------------
	 0        Black
	 1        Red
	 2        Green
	 3        Brown
	 4        Blue
	 5        Purple
	 6        Cyan
	 7        Gray
	 */
	printf("%c[%dm", ESC, background + 40);
}



void color(uint8_t foreground, uint8_t background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
	uint8_t type = 22;             // normal text
	if (foreground > 7) {
		type = 1;                // bold text
		foreground -= 8;
	}
	printf("%c[%d;%d;%dm", ESC, type, foreground + 30, background + 40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
	printf("%c[m", ESC);
}

void clrscr() {
	printf("%c[2J%c[H", ESC, ESC);
}

void clreol() {
	printf("%c[1F%c[2K", ESC, ESC);
}

void gotoxy(uint16_t x, uint16_t y) {
	printf("%c[%d;%dH", ESC, y, x); // line = y, column = x
}

void underline(uint8_t on) {
	if (on % 2 == 0) { // False
		printf("%c[24m", ESC);
	} else { // True
		printf("%c[4m", ESC);
	}
}

void blink(uint8_t on) {
	if (on % 2 == 0) { // False
		printf("%c[25m", ESC);
	} else { // True
		printf("%c[5m", ESC);
	}
}

void inverse(uint8_t on) {
	if (on % 2 == 0) { // False
		printf("%c[27m", ESC);
	} else { // True
		printf("%c[7m", ESC);
	}
}
void lcd_write_string(uint8_t buffer[512], char *slice, uint8_t line) {

	uint8_t location = 0;

	for (int i = 0; i < strlen(slice); i++) {
		for (int j = 0; j < 5; j++) {
			buffer[location + j + (line - 1) * 128] = character_data[slice[i] - 32][j];
		}
		location += 5;
	}
	lcd_push_buffer(buffer);
}

void lcd_write_string2(uint8_t buffer[512], char *slice, uint8_t line) {

	uint8_t location = 65;

	for (int i = 0; i < strlen(slice); i++) {
		for (int j = 0; j < 5; j++) {
			buffer[location + j + (line - 1) * 128] = character_data[slice[i] - 32][j];
		}
		location += 5;
	}
	lcd_push_buffer(buffer);
}
int32_t expand(int32_t i) {
	// Converts an 18.14 fixed point number to 16.16
	return i << 2;
}

int32_t calcSin(int32_t i) {
	i = (i * 512) / 360;
	if (i < 0) {
		int temp_i = -i;
	return -expand(SIN[temp_i]);
	} else if (i > 512) {
		i = i % 512;
	}
	return expand(SIN[i]);
}

int32_t calcCos(int32_t i) {
	i += 90;
	return calcSin(i);
}



void printFix(int32_t i) {
	// Prints a signed 16.16 fixed point number
	if ((i & 0x80000000) != 0) { // Handle negative numbers
		printf("-");
		i = ~i + 1;
	}
	printf("%ld.%04ld", i >> 16, 10000 * (uint32_t) (i & 0xFFFF) >> 16);
	// Print a maximum of 4 decimal digits to avoid overflow
}



/*void rotateVector(struct vector *v, int32_t degree) {
	int32_t tempX;
	tempX = v->x * calcCos(degree) - v->y * calcSin(degree);
	v->y = v->x * calcSin(degree) + v->y * calcCos(degree);
	v->x = tempX;
}*/

void turnOn(GPIO_TypeDef *pin, uint32_t pinnum) {
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C

	pin->OSPEEDR &= ~(0x00000003 << (pinnum * 2)); // Clear speed register
	pin->OSPEEDR |= (0x00000002 << (pinnum * 2)); // set speed register (0x01 - 10
	pin->OTYPER &= ~(0x0001 << (pinnum * 1)); // Clear output type register
	pin->OTYPER |= (0x0000 << (pinnum)); // Set output type register (0x00 -
	pin->MODER &= ~(0x00000003 << (pinnum * 2)); // Clear mode register
	pin->MODER |= (0x00000001 << (pinnum * 2)); // Set mode register (0x00 –

	pin->ODR &= ~(0x0001 << pinnum); //Set pin to low (turned on)

}

void turnOff(GPIO_TypeDef *pin, uint32_t pinnum) {

	pin->OSPEEDR &= ~(0x00000003 << (pinnum * 2)); // Clear speed register
	pin->OSPEEDR |= (0x00000002 << (pinnum * 2)); // set speed register (0x01 - 10
	pin->OTYPER &= ~(0x0001 << (pinnum * 1)); // Clear output type register
	pin->OTYPER |= (0x0000 << (pinnum)); // Set output type register (0x00 -
	pin->MODER &= ~(0x00000003 << (pinnum * 2)); // Clear mode register
	pin->MODER |= (0x00000001 << (pinnum * 2)); // Set mode register (0x00 –

	pin->ODR |= (0x0001 << pinnum); //Set pin to high (turned off)
}


