#include "shapes.h"

ShapesUnion MakeBox(int16_t x1, int16_t y1, int16_t width, int16_t height, int8_t r, int8_t g, int8_t b, int8_t a)
{
	ShapeBox* box = malloc(sizeof(ShapeBox));
	box->x1 = x1;
	box->y1 = y1;
	box->width = width;
	box->height = height;
	box->r = r;
	box->g = g;
	box->b = b;
	box->a = a;
	
	ShapesUnion shape;
	shape.type = BOX;
	shape.box = box;
	return shape;
}	//Must be paired with a free(ShapeRectangle*) always.

ShapesUnion MakeCircle(int16_t xC, int16_t yC, int16_t rad, int8_t r, int8_t g, int8_t b, int8_t a)
{
	ShapeCircle* circle = malloc(sizeof(ShapeCircle));
	circle->xC = xC;
	circle->yC = yC;
	circle->rad = rad;
	circle->r = r;
	circle->g = g;
	circle->b = b;
	circle->a = a;
	
	ShapesUnion shape;
	shape.type = CIRCLE;
	shape.circle = circle;
	return shape;
}	//Must be paired with a free(ShapeCircle*) always.

void RenderShape(ShapesUnion shape)
{
	if(shape.type == BOX)
		boxRGBA(gRenderer, shape.box->x1, shape.box->y1, shape.box->width + shape.box->x1, shape.box->height + shape.box->y1,
			shape.box->r, shape.box->g, shape.box->b, shape.box->a);	//TO DO: Could change to use directly the render rectangle function of SDL!
	else if(shape.type == CIRCLE)
		filledCircleRGBA(gRenderer, shape.circle->xC, shape.circle->yC, shape.circle->rad, 
			shape.circle->r, shape.circle->g, shape.circle->b, shape.circle->a);
}

void DestroyShape(ShapesUnion shape)
{
	if(shape.type == BOX)
		free(shape.box);
	else if(shape.type == CIRCLE)
		free(shape.circle);
	
	shape.type = NONE;
}

int8_t CheckCollision(ShapesUnion shapeA, ShapesUnion shapeB)
{
	int8_t collided = 0;
	if(shapeA.type == BOX && shapeB.type == BOX)
	{
		ShapeBox* a_box = shapeA.box;
		ShapeBox* b_box = shapeB.box;
		
		int16_t a_left 		= a_box->x1;
		int16_t a_right 	= a_left + a_box->width;
		int16_t a_top		= a_box->y1;
		int16_t a_bottom	= a_top + a_box->height;

		int16_t b_left 		= b_box->x1;
		int16_t b_right 	= b_left + b_box->width;
		int16_t b_top		= b_box->y1;
		int16_t b_bottom	= b_top + b_box->height;
		
		if(a_right > b_left && a_left < b_right &&  a_bottom > b_top && a_top < b_bottom)
			collided = 1;	//This means true.
	}
	
	return collided;
}
