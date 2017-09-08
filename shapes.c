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


//int16_t* pX, int16_t* pY, int8_t pN, can be passed as NULL, NULL and 0, and in that case, an "EMPTY POLYGON" is made.
ShapesUnion MakePoly(int16_t* pX, int16_t* pY, int8_t pN, int8_t filled, int8_t r, int8_t g, int8_t b, int8_t a)
{
	ShapePoly* poly = malloc(sizeof(ShapePoly));
	
	if(pX == NULL || pY == NULL || pN == 0)
	{
		poly->pX = NULL;
		poly->pY = NULL;
		pN = 0;
	}
	else
	{
		poly->pX = malloc(sizeof(int16_t) * pN);
		poly->pY = malloc(sizeof(int16_t) * pN);
		
		for(int8_t i = 0; i < pN; i++)		//Copy array of points passed to array of points pointed to in the struct.
		{
			poly->pX[i] = pX[i];
			poly->pY[i] = pY[i];
		}
	}
	
	poly->pN = pN;
	poly->filled = filled;
	poly->r = r;
	poly->g = g;
	poly->b = b;
	poly->a = a;
	
	ShapesUnion shape;
	shape.type = POLY;
	shape.poly = poly;
	return shape;
}//Can be a polygon of less than three points, and in that case, only points should be drawn, if there are any.
//Must be paired with a free(ShapePoly*) always.

void AddPointToPoly(ShapePoly* p, int16_t x, int16_t y)
{
	int8_t pN = p->pN + 1;
	p->pX = realloc(p->pX, sizeof(int16_t) * pN);
	p->pY = realloc(p->pY, sizeof(int16_t) * pN);
	
	p->pX[p->pN] = x;
	p->pY[p->pN] = y;
	p->pN = pN;
}

void RenderShape(ShapesUnion shape)
{
	if(shape.type == BOX)
		boxRGBA(gRenderer, shape.box->x1, shape.box->y1, shape.box->width + shape.box->x1, shape.box->height + shape.box->y1,
			shape.box->r, shape.box->g, shape.box->b, shape.box->a);	//TO DO: Could change to use directly the render rectangle function of SDL!
	else if(shape.type == CIRCLE)
		filledCircleRGBA(gRenderer, shape.circle->xC, shape.circle->yC, shape.circle->rad, 
			shape.circle->r, shape.circle->g, shape.circle->b, shape.circle->a);
	else if(shape.type == POLY)
	{
		if(shape.poly->pN >= 3)
			polygonRGBA(gRenderer, shape.poly->pX, shape.poly->pY, shape.poly->pN, 
				shape.poly->r, shape.poly->g, shape.poly->b, shape.poly->a);
	}
		
}

void DestroyShape(ShapesUnion shape)
{
	if(shape.type == BOX)
		free(shape.box);
	else if(shape.type == CIRCLE)
		free(shape.circle);
	else if(shape.type == POLY)
		free(shape.poly);
	
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
	else
	{
		ShapesUnion * a, * b;
		if(shapeA.type == BOX)
		{
			a = &shapeA;
			b = &shapeB;
		}
		else if(shapeB.type == BOX)
		{
			a = &shapeB;
			b = &shapeA;
		}
		
		if(a->type == BOX && b->type == CIRCLE)
		{
			ShapeBox* a_box = a->box;
			ShapeCircle* b_cir = b->circle;
			
			int16_t a_left 		= a_box->x1;
			int16_t a_right 	= a_left + a_box->width;
			int16_t a_top		= a_box->y1;
			int16_t a_bottom	= a_top + a_box->height;

			int16_t b_xCenter	= b_cir->xC;
			int16_t b_yCenter 	= b_cir->yC;
			int16_t b_rad		= b_cir->rad;
			
			//https://yal.cc/rectangle-circle-intersection-test/ :
			int16_t min = (b_xCenter < a_right) ? b_xCenter : a_right;
			int16_t max = (a_left > min) ? a_left : min;
			int16_t deltaX = b_xCenter - max;//(a_left, min(b_xCenter, a_right));
			
			min = (b_yCenter < a_bottom) ? b_yCenter : a_bottom;
			max = (a_top > min) ? a_top : min;
			int16_t deltaY = b_yCenter - max;//(a_top, min(b_yCenter, a_bottom));
			
			return (deltaX * deltaX + deltaY * deltaY) < (b_rad * b_rad);
		}
	}
	
	return collided;
}
