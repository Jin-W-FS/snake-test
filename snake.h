#ifndef _SNAKE_STRUCT_H_
#define _SNAKE_STRUCT_H_

/* usual snake */
typedef struct _Point
{
	int x, y;
}Point;

#define MAP_WIDTH 80
#define MAP_HEIGHT 24

extern char base_map[MAP_HEIGHT][MAP_WIDTH];

typedef struct _Food
{
	Point point;
	float weight;
}Food;

enum DIRECT{
	EAST, SOUTH, WEST, NORTH
};

#define MAX_SNAKE_LEN (MAP_WIDTH * MAP_HEIGHT)
typedef struct _Snake
{
	Point* snake;
	int length;		/* if length < 0, game over */
	int len_to_grow;
	enum DIRECT direct;
}Snake;

void init(Snake* snake, Food* food);
void release(Snake* snake);

void move(Snake* snake);
void gainFood(Food* food);
/* check snake ate food OR crashed */
void check(Snake* snake, Food* food);

#define PRT_BLANK ' '
#define PRT_WALL '#'
#define PRT_FOOD '@'
#define PRT_SNAKE '*'
void draw_to_map(Snake* snake, Food* food);
void print();			/* really paint to screen */



#endif
