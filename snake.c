#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>

#include "snake.h"

void draw_to_map(Map map, Map base, Snake* snake, Food* food)
{
	static char* gameover = "GAME OVER";
	int i;
	/* base map */
	memcpy(map, base, MAP_WIDTH * MAP_HEIGHT);
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
void check(SnakeWorld* world)
{
	Food* food = &(world->food);
	Snake* snake = &(world->snake);
	Point* head = snake->snake;
	if (world->map[head->y][head->x] == PRT_WALL || world->map[head->y][head->x] == PRT_SNAKE) /* crashed */
		snake->length = -1;
	else if (head->x == food->point.x && head->y == food->point.y) /* ate food */
	{
		snake->len_to_grow += food->weight;
		gain_food(world->map, food);
	}
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
	assert(snake->direct >= 0 && snake->direct < 4);

	memmove(snake->snake + 1, snake->snake, snake->length * sizeof(Point));
	snake->snake[0].x += p_direct[snake->direct].x;
	snake->snake[0].y += p_direct[snake->direct].y;
	if (snake->len_to_grow > 0)
	{
		snake->len_to_grow--;
		snake->length++;
	}
}
/* WOS: World Of Snake */
void init(SnakeWorld* world)
{
	int i, j;
	Food* food = &(world->food);
	Snake* snake = &(world->snake);

	/* init base_map */
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == MAP_HEIGHT - 1 || j == MAP_WIDTH - 1)
				world->base_map[i][j] = PRT_WALL;
			else
				world->base_map[i][j] = PRT_BLANK;
		}
	}
	/* snake @ (1,1) */
	snake->snake = (Point*)malloc(MAX_SNAKE_LEN * sizeof(Point));
	snake->length = 1;
	snake->len_to_grow = 3;
	snake->direct = EAST;
	snake->snake[0].x = snake->snake[0].y = 1;
	/* init map for init food */
	draw_to_map(world->map, world->base_map, snake, NULL);
	/* init food */
	srand((unsigned int)time(NULL));
	gain_food(world->map, food);

	world->print = NULL;
	world->setdir = NULL;
}
void release(SnakeWorld* world)
{
	free(world->snake.snake);
	world->snake.snake = NULL;
	world->print = NULL;
	world->setdir = NULL;
}

int run_once(SnakeWorld* world)
{
	move(&(world->snake));
	check(world);
	draw_to_map(world->map, world->base_map, &world->snake, &world->food);
	
	return (world->snake.length > 0);
}

int run(SnakeWorld* world)
{
	if (world->print)
		(*(world->print))(world->map);
	if (world->setdir)
		(*(world->setdir))(world);

	while(run_once(world))
	{
		if (world->print)
			(*(world->print))(world->map);
		if (world->setdir)
			(*(world->setdir))(world);
	}
	if (world->print)
		(*(world->print))(world->map);
	return 0;
}


	
	
