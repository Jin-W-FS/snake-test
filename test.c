#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "snake.h"


int main()
{
	Snake snake;
	Food food;
	init(&snake, &food);
	food.point.x = 9, food.point.y = 1;
	
	while(snake.length > 0)
	{
		draw_to_map(&snake, &food);
		print();
		move(&snake);
		check(&snake, &food);
	}
	draw_to_map(&snake, &food);
	print();
		
	release(&snake);
	
	return 0;
}
