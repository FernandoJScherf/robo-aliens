#include "shapes.h"

ShapesUnion MakeBox(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t r, int8_t g, int8_t b, int8_t a)
{
	ShapeBox* box = malloc(sizeof(ShapeBox));
	box->x1 = x1;
	box->y1 = y1;
	box->x2 = x2;
	box->y2 = y2;
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
		boxRGBA(gRenderer, shape.box->x1, shape.box->y1, shape.box->x2, shape.box->y2,
			shape.box->r, shape.box->g, shape.box->b, shape.box->a);
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
	
}
