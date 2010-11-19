#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* for sleep() */
#include <unistd.h>

#include "snake.h"

#define ENG_MIN -1e37f
#define ENG_MAX +1e37f

int print(Map map)
{
	int i;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		putchar('\n');
		write(STDOUT_FILENO, map[i], MAP_WIDTH);
	}
 	sleep(0);
}

void init_ctrl(SnakeWorld* world);
int set_dir(SnakeWorld* world);

int demo()
{
	SnakeWorld world;
	init(&world);
	world.print = print;
	world.setdir = set_dir;
	world.food.point.x = 5, world.food.point.y = 3;

	init_ctrl(&world);
	
	run(&world);
	release(&world);
	return 0;
}

int main()
{
	demo();
	return 0;
}

/* energy field */
float base_energy_field[MAP_HEIGHT][MAP_WIDTH];
float energy_field[MAP_HEIGHT][MAP_WIDTH];

float distance_of(Point a,Point b)
{
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
float energy_of(Point a, Point b, float weight, int is_gravity)
{
	float e;
	if (Point_eq(a,b))
		return (is_gravity ? ENG_MIN : ENG_MAX);
	else
	{
		e = weight / distance_of(a, b);
		return (is_gravity ? -e : e);
	}
}

void init_ctrl(SnakeWorld* world)
{
	/* now just init inner energy_field to zero, and the wall to ENG_MAX */
	int i, j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			if (world->base_map[i][j] == PRT_WALL)
				base_energy_field[i][j] = ENG_MAX;
		}
	}
}

int set_dir(SnakeWorld* world)
{
	Snake* snake = &(world->snake);
	Food* food = &(world->food);
	
	Point near[4];
	float engy[4];
	
	int i, j, min_id;
	if (snake->length <= 0)
		return -1;
	for (i = 0; i < 4; i++)
	{
		near[i].x = snake->snake[0].x + p_direct[i].x; 
		near[i].y = snake->snake[0].y + p_direct[i].y;
		engy[i] = base_energy_field[near[i].y][near[i].x];

		/* attraction force */
		engy[i] += energy_of(near[i], food->point, food->weight * 500, 1);

		for (j = 1; j < snake->length; j++)
		{		/* repulsion force */
			if (Point_eq(near[i], snake->snake[j]))
			{
				engy[i] = ENG_MAX;
				break;
			}
			else
				engy[i] += energy_of(near[i], snake->snake[j], 1, 0);
		}
		if (engy[i] > ENG_MAX) engy[i] = ENG_MAX;
		if (engy[i] < ENG_MIN) engy[i] = ENG_MIN;
	}
	
	/* towards the min energy direct */
	min_id = 0;
	for (i = 1; i < 4; i++)
	{
		if (engy[i] < engy[min_id])
			min_id = i;
	}
	snake->direct = min_id;
	return min_id;
}
