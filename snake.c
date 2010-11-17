#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "snake.h"

#ifndef FLT_MIN
#	define FLT_MIN 1e-37f
#endif

#ifndef FLT_MAX
#	define FLT_MAX 1e+37f
#endif

Map base_map;
static char* gameover = "GAME OVER";

void draw_to_map(Map map, Snake* snake, Food* food)
{
	int i;
	/* base map */
	memcpy(map, base_map, MAP_WIDTH * MAP_HEIGHT);
	/* food, may be overlapped by snake */
	if (food)
		map[food->point.y][food->point.x] = PRT_FOOD;
	/* snake */
	if (snake)
	{
		if (snake->length < 0){	/* game over */
			memcpy(&map[MAP_HEIGHT / 2][MAP_WIDTH / 2 - 5], gameover, 9);
		}
		for (i = 0; i < snake->length; i++)
		{
			map[snake->snake[i].y][snake->snake[i].x] = PRT_SNAKE;
		}
	}
}

void gain_food(Map map, Food* food)
{
	do
	{
		food->point.x = rand() % MAP_WIDTH;
		food->point.y = rand() % MAP_HEIGHT;
	}while(map[food->point.y][food->point.x] != PRT_BLANK);
	food->weight = 1;
}

/* check snake ate food OR crashed */
void check(Map map, Snake* snake, Food* food)
{
	Point* head = snake->snake;
	if (map[head->y][head->x] == PRT_WALL || map[head->y][head->x] == PRT_SNAKE) /* crashed */
		snake->length = -1;
	else if (head->x == food->point.x && head->y == food->point.y) /* ate food */
	{
		snake->len_to_grow += food->weight;
		gain_food(map, food);
	}
}


void init(Map map, Snake* snake, Food* food)
{
	int i, j;
	/* init base_map */
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == MAP_HEIGHT - 1 || j == MAP_WIDTH - 1)
				base_map[i][j] = PRT_WALL;
			else
				base_map[i][j] = PRT_BLANK;
		}
	}
	/* snake @ (1,1) */
	snake->snake = (Point*)malloc(MAX_SNAKE_LEN * sizeof(Point));
	snake->length = 1;
	snake->len_to_grow = 3;
	snake->direct = EAST;
	snake->snake[0].x = snake->snake[0].y = 1;
	/* init map for init food */
	draw_to_map(map, snake, NULL);
	/* init food */
	srand((unsigned int)time(NULL));
	gain_food(map, food);
}
void release(Snake* snake)
{
	free(snake->snake);
	snake->snake = NULL;
}

/* move snake towards snake->direct */
Point p_direct[4] =
{
	{ 1  , 0  },		/* EAST  */
	{ 0  , 1  },		/* SOUTH */
	{ -1 , 0  },		/* WEST  */
	{ 0  , -1 },		/* NORTH */
};
	
void move(Snake* snake)
{
	assert(snake->length > 0);
	assert(snake->len_to_grow >= 0);
	
	memmove(snake->snake + 1, snake->snake, snake->length * sizeof(Point));
	snake->snake[0].x += p_direct[snake->direct].x;
	snake->snake[0].y += p_direct[snake->direct].y;
	if (snake->len_to_grow)
	{
		snake->len_to_grow--;
		snake->length++;
	}
}

int run(print_func print, setDirect_func set_direct)
{
	Snake snake;
	Food food;
	Map map;
	
	init(map, &snake, &food);
	food.point.x = 9, food.point.y = 1;
	
	while(snake.length > 0)
	{
		draw_to_map(map, &snake, &food);
		if (print)
			print(map);
		if (set_direct)
			set_direct(map, &snake, &food);
		move(&snake);
		check(map, &snake, &food);
	}
	draw_to_map(map, &snake, &food);
	if (print)
		print(map);
		
	release(&snake);
	
	return 0;
}
