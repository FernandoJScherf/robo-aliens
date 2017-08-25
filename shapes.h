#include "rob.h"

typedef enum
{
	NONE,
	BOX,
	CIRCLE,
	POLYGON
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
	ShapesEnum			type;
	union
	{
		ShapeBox*			box;
		ShapeCircle*		circle;
	};
}ShapesUnion;

ShapesUnion MakeBox(int16_t x1, int16_t y1, int16_t width, int16_t height, int8_t r, int8_t g, int8_t b, int8_t a);
//Must be paired with a free(ShapeRectangle*) always.

ShapesUnion MakeCircle(int16_t xC, int16_t yC, int16_t rad, int8_t r, int8_t g, int8_t b, int8_t a);
//Must be paired with a free(ShapeCircle*) always.

void RenderShape(ShapesUnion shape);

void DestroyShape(ShapesUnion shape);

int8_t CheckCollision(ShapesUnion shapeA, ShapesUnion shapeB);
