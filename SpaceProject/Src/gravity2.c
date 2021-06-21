#include "ansi.h"
#include "GameController.h"
#include "main.h"
#include "math.h"
#include "fpsqrt.h"


#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )
#define G 1 << 6

void gravity(uint32_t m1, uint32_t m2, struct bullet *bulletptr, int64_t a, int64_t b) {

	int32_t x1 = bulletptr->pos.x, y1 = bulletptr->pos.y;
	int64_t x = a - x1 , y = b - y1;
	int32_t r = FIX14_MULT(x, x) + FIX14_MULT(y, y);
	int64_t r2 = (sqrt_i32_to_fx16_16(r>>14))>>2;
	//gotoxy(10,10);


	int64_t f = FIX14_MULT(G,FIX14_DIV(FIX14_MULT((int64_t)m1,(int64_t)m2),r));

	int64_t ay = FIX14_DIV(FIX14_MULT(FIX14_DIV(y,r2),f),(int64_t)m1);
	int64_t ax = FIX14_DIV(FIX14_MULT(FIX14_DIV(x,r2),f),(int64_t)m1);
	bulletptr->vel.x += ax;
	bulletptr->vel.y += ay;

	if (bulletptr->vel.x>>14 >= 3){
		bulletptr->vel.x = 2<<14;
	}
	if (bulletptr->vel.y>>14 >= 3){
			bulletptr->vel.y = 2<<14;
		}
}



