#ifndef _SNAKE_STRUCT_H_
#define _SNAKE_STRUCT_H_

/* the structs to describe an usual snake game */

typedef struct _Point
{
	int x, y;
}Point;

#define MAP_WIDTH 80
#define MAP_HEIGHT 24

typedef char Map[MAP_HEIGHT][MAP_WIDTH];
extern Map base_map;

typedef struct _Food
{
	Point point;
	float weight;
}Food;

enum DIRECT{
	EAST, SOUTH, WEST, NORTH
};
extern Point p_direct[4];

#define MAX_SNAKE_LEN (MAP_WIDTH * MAP_HEIGHT)
typedef struct _Snake
{
	Point* snake;
	int length;		/* if length < 0, game over */
	int len_to_grow;
	enum DIRECT direct;
}Snake;

void init(Map map, Snake* snake, Food* food);
void release(Snake* snake);

void move(Snake* snake);
void gain_food(Map map, Food* food);
/* check snake ate food OR crashed */
void check(Map map, Snake* snake, Food* food);

#define PRT_BLANK ' '
#define PRT_WALL '#'
#define PRT_FOOD '@'
#define PRT_SNAKE '*'
void draw_to_map(Map map, Snake* snake, Food* food);

/* callback functions */
typedef int (*print_func)(Map map);
typedef int (*setDirect_func)(Map map, Snake* snake, Food* food);

int run(print_func print, setDirect_func set_direct);

#endif
