#include "rob.h"

typedef enum
{
	NONE,
	BOX,
	CIRCLE,
	POLY
}ShapesEnum;

typedef struct
{
	int16_t				x1;
	int16_t				y1;
	int16_t				width;
	int16_t				height;
	int8_t				r;
	int8_t				g;
	int8_t				b;
	int8_t 				a;
} ShapeBox;

typedef struct
{
	int16_t				xC;
	int16_t				yC;
	int16_t				rad;
	int8_t				r;
	int8_t				g;
	int8_t				b;
	int8_t 				a;
}ShapeCircle;

typedef struct
{
	int16_t*			pX;
	int16_t*			pY;
	int8_t 				pN;			//Total of points.
	int8_t				filled;		//If filled or not. 1 is filled, 0 is not. TO DO: Maybe they are all filled and this is not necessary.
	int8_t				r;
	int8_t				g;
	int8_t				b;
	int8_t 				a;
}ShapePoly;
//TO DO: Why not put the r g b a members inside of ShapesUnion instead of repeating them in every shape struct!?!?!?!
//What the fuck was I thinking???

typedef struct
{
	ShapesEnum			type;
	union
	{
		ShapeBox*			box;
		ShapeCircle*		circle;
		ShapePoly*			poly;
	};
}ShapesUnion;

ShapesUnion MakeBox(int16_t x1, int16_t y1, int16_t width, int16_t height, int8_t r, int8_t g, int8_t b, int8_t a);
//Must be paired with a free(ShapeRectangle*) always.

ShapesUnion MakeCircle(int16_t xC, int16_t yC, int16_t rad, int8_t r, int8_t g, int8_t b, int8_t a);
//Must be paired with a free(ShapeCircle*) always.

ShapesUnion MakePoly(int16_t* pX, int16_t* pY, int8_t pN, int8_t filled, int8_t r, int8_t g, int8_t b, int8_t a);
//Can be a polygon of less than three points, and in that case, only points should be drawn, if there are any.
//Must be paired with a free(ShapePoly*) always.

void AddPointToPoly(ShapePoly* p, int16_t x, int16_t y);

void RenderShape(ShapesUnion shape);

void DestroyShape(ShapesUnion shape);

int8_t CheckCollision(ShapesUnion shapeA, ShapesUnion shapeB);
