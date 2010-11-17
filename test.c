#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* for sleep() */
#include <unistd.h>

#include "snake.h"

#ifndef FLT_MIN
#	define FLT_MIN 1e-37f
#endif

#ifndef FLT_MAX
#	define FLT_MAX 1e+37f
#endif

int print(Map map)
{
	int i;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		putchar('\n');
		write(STDOUT_FILENO, map[i], MAP_WIDTH);
	}
	sleep(1);
}

int set_dir(SnakeWorld* world)
{
	static int cnt;
	world->snake.direct = cnt % 40 / 10;
	cnt++;
}

int demo()
{
	SnakeWorld world;
	init(&world);
	world.print = print;
	world.setdir = set_dir;
	world.food.point.x = 11, world.food.point.y = 3;
	run(&world);
	release(&world);
	return 0;
}

int main()
{
	demo();
	return 0;
}

