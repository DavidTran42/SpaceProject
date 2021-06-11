#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

typedef struct vector_t {
	int32_t x, y;
} vector_t;

typedef struct ship {
	vector_t position;
} ship;

void test(){
	clrscr();
	struct ship ship1;
		ship1.position.x = 5, ship1.position.y = 5;
		print_ship(&ship1.position);

}

int main(void)
{
	test();

	while(1){}
}
