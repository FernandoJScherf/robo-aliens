#include "shapes.h"

#define MAX_SHAPES 500

#define UNI SCREEN_WIDTH / 64

StatesEnum UpdateAndRenderObjectEditor(dT)
{
	static TTF_Font* textFont;
	static Text* textShapes;
	
	static SubStatesEnum subState = Load;
	
	static ShapesUnion* shapeArray;
	
	if(subState == Load)
	{
		subState = Normal;		
		textFont = LoadFont("synchronizer_nbp.ttf", 16);
		SDL_Color textColor = { 0, 0, 0, 255 };	
		textShapes = MakeText("Shapes", textFont, textColor, UNI * 53, UNI);
		
		shapeArray = calloc(MAX_SHAPES, sizeof(ShapesUnion));
		//Every member of every element in shapeArray should be initialized to zero.
		
		//Make interface:
		shapeArray[0] = MakeBox(UNI * 54, UNI * 4, UNI * 56, UNI * 6, 127, 0, 0, 255);
		shapeArray[1] = MakeCircle(UNI * 55, UNI * 8, UNI, 0, 127, 0, 255);
	}
	
	////////Every Frame:
	
	//Render Interface:
	RenderText(textShapes);
	vlineRGBA(gRenderer, UNI * 52, 0, SCREEN_HEIGHT, 0, 0, 0, 255);
	for(int i = 0; i < MAX_SHAPES; i++)
	{
		if(shapeArray[i].type != NONE)
			RenderShape(shapeArray[i]);		//If we found an occupied space in the array.
	}
	
	if(subState == Exit)
	{
		TTF_CloseFont(textFont);
		DestroyText(textShapes);
		for(int i = 0; i < MAX_SHAPES; i++)
		{
			if(shapeArray[i].type != NONE)
				DestroyShape(shapeArray[i]);
		}
		free(shapeArray);
		
		textFont = NULL;
		textShapes = NULL;
		shapeArray = NULL;
		
		subState = Load;		
	}
	
	return ObjectEditor;
}


//~ #include "rob.h"

//~ //An array in wich each element points to a particular strcut of type circle, rect, poly, etc.

//~ #define MAX_SHAPES 500

//~ typedef struct
//~ {
	//~ int16_t				x1;
	//~ int16_t				y1;
	//~ int16_t				x2;
	//~ int16_t				y2;
	//~ int8_t				r;
	//~ int8_t				g;
	//~ int8_t				b;
	//~ int8_t 				a;
//~ } ShapeBox;

//~ typedef struct
//~ {
	//~ int16_t				xC;
	//~ int16_t				yC;
	//~ int16_t				rad;
	//~ int8_t				r;
	//~ int8_t				g;
	//~ int8_t				b;
	//~ int8_t 				a;
//~ }ShapeCircle;

//~ ShapeBox* MakeBox(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t r, int8_t g, int8_t b, int8_t a)
//~ {
	//~ ShapeBox* box = malloc(sizeof(ShapeBox));
	//~ box->x1 = x1;
	//~ box->y1 = y1;
	//~ box->x2 = x2;
	//~ box->y2 = y2;
	//~ box->r = r;
	//~ box->g = g;
	//~ box->b = b;
	//~ box->a = a;
	//~ return box;
//~ }	//Must be paired with a free(ShapeRectangle*) always.

//~ void RenderBox(ShapeBox* box)
//~ {
	//~ boxRGBA(gRenderer, box->x1, box->y1, box->x2, box->y2, box->r, box->g, box->b, box->a);
//~ }

//~ ShapeCircle* MakeCircle(int16_t xC, int16_t yC, int16_t rad, int8_t r, int8_t g, int8_t b, int8_t a)
//~ {
	//~ ShapeCircle* circle = malloc(sizeof(ShapeCircle));
	//~ circle->xC = xC;
	//~ circle->yC = yC;
	//~ circle->rad = rad;
	//~ circle->r = r;
	//~ circle->g = g;
	//~ circle->b = b;
	//~ circle->a = a;
	//~ return circle;
//~ }	//Must be paired with a free(ShapeCircle*) always.

//~ void RenderCircle(ShapeCircle* circle)
//~ {
	//~ filledCircleRGBA(gRenderer, circle->xC, circle->yC, circle->rad, circle->r, circle->g, circle->b, circle->a);
//~ }

//~ #define UNI SCREEN_WIDTH / 64

//~ StatesEnum UpdateAndRenderObjectEditor(dT)
//~ {
	//~ static TTF_Font* textFont;
	//~ static Text* textShapes;
	
	//~ static SubStatesEnum subState = Load;
	
	//~ static ShapeBox* selBox;
	//~ static ShapeCircle* selCircle;
	
	//~ if(subState == Load)
	//~ {
		//~ subState = Normal;		
		//~ textFont = LoadFont("synchronizer_nbp.ttf", 16);
		//~ SDL_Color textColor = { 0, 0, 0, 255 };	
		//~ textShapes = MakeText("Shapes", textFont, textColor, UNI * 53, UNI);
		
		//~ //Make interface:
		//~ selBox = MakeBox(UNI * 54, UNI * 4, UNI * 56, UNI * 6, 127, 0, 0, 255);
		//~ selCircle = MakeCircle(UNI * 55, UNI * 8, UNI, 0, 127, 0, 255);
		
	//~ }
	
	//~ ////////Every Frame:
	
	
	//~ //Render Interface:
	//~ RenderText(textShapes);
	//~ RenderBox(selBox);
	//~ RenderCircle(selCircle);
	//~ vlineRGBA(gRenderer, UNI * 52, 0, SCREEN_HEIGHT, 0, 0, 0, 255);
	
	//~ return ObjectEditor;
	
	//~ if(subState == Exit)
	//~ {
		//~ TTF_CloseFont(textFont);
		//~ DestroyText(textShapes);
		//~ free(selBox);
		//~ free(selCircle);
	//~ }
//~ }
