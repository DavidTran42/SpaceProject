#include "ansi.h"
#include "GameController.h"
#include "main.h"
#include "math.h"
#include "fpsqrt.h"

#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )

void gravity(struct bullet *bulletptr, struct asteroid *asteroidptr) {

	int64_t x1 = bulletptr->pos.x, x2 = asteroidptr->pos.x;
	int64_t y1 = bulletptr->pos.y, y2 = asteroidptr->pos.y;
	int64_t a = x1 - x2, b = y1 - y2;
	int64_t r = FIX14_MULT(a, a) + FIX14_MULT(b, b);
	int64_t r2 = sqrt_i64(r);
	int64_t ex = FIX14_DIV(a, r2), ey = FIX14_DIV(b, r2);
	int64_t f = 0, ax = 0, ay = 0;

	if (asteroidptr->size == 2) {
		f = FIX14_DIV(2, r);
		ax = f * ex;
		ay = f * ey;

		bulletptr->vel.x += FIX14_DIV(ax, 1);
		bulletptr->vel.y += FIX14_DIV(ay, 1);

	}
	if (asteroidptr->size == 4) {
		f = FIX14_DIV(2, r);
		ax = f * ex;
		ay = f * ey;

		bulletptr->vel.x += FIX14_DIV(ax, 1);
		bulletptr->vel.y += FIX14_DIV(ay, 1);

	}
	if (asteroidptr->size == 8) {
		f = FIX14_DIV(2, r);
		ax = f * ex;
		ay = f * ey;

		bulletptr->vel.x += FIX14_DIV(ax, 1);
		bulletptr->vel.y += FIX14_DIV(ay, 1);

	}
	if (bulletptr->pos.x > 260 || bulletptr->pos.y > 70) {
		bulletptr->vel.x = 0;
		bulletptr->vel.y = 0;
	}
}
