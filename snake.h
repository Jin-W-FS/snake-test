#ifndef _SNAKE_STRUCT_H_
#define _SNAKE_STRUCT_H_

/* the structs to describe an usual snake game */

/* point */
typedef struct _Point
{
	int x, y;
}Point;

#define MAP_WIDTH 80
#define MAP_HEIGHT 24

/* map */
typedef char Map[MAP_HEIGHT][MAP_WIDTH];

/* food */
typedef struct _Food
{
	Point point;
	float weight;
}Food;

/* snake */
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

void move(Snake* snake);
void gain_food(Map map, Food* food);

#define PRT_BLANK ' '
#define PRT_WALL '#'
#define PRT_FOOD '@'
#define PRT_SNAKE '*'
void draw_to_map(Map map, Map base, Snake* snake, Food* food);

/* world of the snake */

/* callback functions */
struct _SnakeWorld;
typedef int (*print_func)(Map map);
typedef int (*setDirect_func)(struct _SnakeWorld* SnakeWorld);

typedef struct _SnakeWorld
{
	Map map, base_map;
	Snake snake;
	Food food;

	print_func print;
	setDirect_func setdir;
}SnakeWorld;

void init(SnakeWorld* world);
void release(SnakeWorld* world);

/* check if snake ate food, and crashed */
void check(SnakeWorld* world);
/* run the world */
int run_once(SnakeWorld* world);
int run(SnakeWorld* world);

#endif
