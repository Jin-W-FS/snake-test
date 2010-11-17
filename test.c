#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* for sleep() */
#include <unistd.h>

#include "snake.h"


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
int set_dir(Map map, Snake* snake, Food* food)
{
	static int cnt;
	snake->direct = cnt % 20 / 5;
	cnt++;
}

int main()
{
	return run(print, set_dir);
}

