#include "shapes.h"

#define MAX_SHAPES 500

#define UNI SCREEN_WIDTH / 64

typedef enum
{
	NONESELECTED,
	POLYCREATE,
	POLYMODIFY,
	REDMODIFY,
	GREENMODIFY,
	BLUEMODIFY
}SelectedStateEnum;

StatesEnum UpdateAndRenderObjectEditor(double dT)
{
	static TTF_Font* textFont;
	static Text* textShapes;
	
	static SubStatesEnum subState = Load;
	
	static ShapesUnion* shapeArray;
	
	if(subState == Load)
	{
		subState = Normal;		
		textFont = LoadFont("synchronizer_nbp.ttf", 8);
		SDL_Color textColor = { 0, 0, 0, 255 };	
		textShapes = MakeText("Shapes", textFont, textColor, UNI * 53, UNI);
		
		shapeArray = calloc(MAX_SHAPES, sizeof(ShapesUnion));
		//Every member of every element in shapeArray should be initialized to zero.
		
		//Make interface:
		shapeArray[0] = MakeBox(UNI * 54, UNI * 4, UNI * 2, UNI * 2, 127, 0, 0, 255);	//Create Polygon.
		shapeArray[1] = MakeCircle(UNI * 55, UNI * 8, UNI, 0, 127, 0, 255);
		shapeArray[2] = MakeBox(UNI * 58, UNI * 4, UNI * 2, UNI * 2, 127, 127, 0, 255);	//Activate / Deactivate "Snap" between points.
	}
	
	////////Every Frame:
	static SelectedStateEnum selectedState = NONE;
	static int16_t workingOnPoly = 0;
	
	static float totalTimeClick = 0.0;
	
	static int16_t savedX, savedY;
	
	static int8_t snapState = 0;	// 0 is false.
	
	//MOUSE CLICKS:
	if(gMouseStates.left == SDL_PRESSED)	//If just LEFT clicked or if mouse is kept being pressed.
	{
		
		int8_t divisor = 2;
		int16_t uniDiv = UNI / divisor;
		ShapesUnion rectMouse = MakeBox(gMouseStates.x - uniDiv, gMouseStates.y - uniDiv, 
			uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);
		if(totalTimeClick == 0.0)
		{			
			//~ printf("totalTimeClick %f\n", totalTimeClick);
			if(CheckCollision(rectMouse, shapeArray[0]))
			{
				//~ printf("Selected the constructiong polygon tool");
				for(int i = 0; i < MAX_SHAPES; i++)
				{
					if(shapeArray[i].type == NONE)		//If we found an empty space in the array.
					{
						workingOnPoly = i;
						shapeArray[i] = MakePoly(NULL, NULL, 0, 0, 127, 127, 127, 255);
						break;		//I FORGOT TO PUT THIS BREAK WHAT THE FUUUUUUCK.
					}
				}
				selectedState = POLYCREATE;
			}
			else if(CheckCollision(rectMouse, shapeArray[1]))
				selectedState = POLYMODIFY;
			else if(CheckCollision(rectMouse, shapeArray[2]))
				snapState ^= 0b00000001;	//XOR to invert bit. Changes to 1 if 0, changes to 0 if 1.
			else
				if(selectedState == POLYCREATE)
				{
					//~ printf("Click outside of button.\n");
					for(int i = 0; i < MAX_SHAPES; i++)
					{
						//~ printf("shapeArray[i].type == %i\n", shapeArray[i].type);
						if(shapeArray[i].type == NONE)		//If we found an empty space in the array.
						{
							SDL_Color textColor = { 127, 0, 0, 255 };	
							textShapes = MakeText("Constructing Polygon", textFont, textColor, UNI * 53, UNI);
							AddPointToPoly(shapeArray[workingOnPoly].poly, gMouseStates.x, gMouseStates.y);
							int16_t w = uniDiv * divisor;
							int16_t h = w;
							break;
						}
					}
				}
				else if(selectedState == POLYMODIFY)
				{
					//Check collision between the point of the mouse and every polygon.
					//Change workingOnPoly to the index of the poly on shapeArray.
					//Enjoy.
				}
			
		}
		totalTimeClick += dT;
		
		RenderShape(rectMouse);
		DestroyShape(rectMouse);
	}
	else if(gMouseStates.middle == SDL_PRESSED)
	{
		if(totalTimeClick == 0.0)
		{
			savedX = gMouseStates.x;
			savedY = gMouseStates.y;
		}
		if(selectedState == POLYCREATE)
		{
			for(int i = 0; i < shapeArray[workingOnPoly].poly->pN; i++)
			{
				shapeArray[workingOnPoly].poly->pX[i] += gMouseStates.x - savedX;
				shapeArray[workingOnPoly].poly->pY[i] += gMouseStates.y - savedY;
			}
			savedX = gMouseStates.x;
			savedY = gMouseStates.y;
		}
		totalTimeClick += dT;
	}
	else if(gMouseStates.right == SDL_PRESSED)
	{
		if(totalTimeClick == 0.0)
		{
			savedX = gMouseStates.x;
			savedY = gMouseStates.y;
		}
		if(selectedState == POLYCREATE)
		{
			int8_t divisor = 2;
			int16_t uniDiv = UNI / divisor;
			ShapesUnion rectMouse = MakeBox(gMouseStates.x - uniDiv, gMouseStates.y - uniDiv, 
				uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);

			for(int i = 0; i < shapeArray[workingOnPoly].poly->pN; i++)
			{
				ShapesUnion rect = MakeBox(shapeArray[workingOnPoly].poly->pX[i] - uniDiv, 
					shapeArray[workingOnPoly].poly->pY[i] - uniDiv, 
					uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);
				if(CheckCollision(rectMouse, rect))
				{//Ok, now, in case the mouse pointer collided with one of the polygon's points:
					shapeArray[workingOnPoly].poly->pX[i] += gMouseStates.x - savedX;
					shapeArray[workingOnPoly].poly->pY[i] += gMouseStates.y - savedY;
					
					//Now that I'm moving one of the points individually, I will see if it's close to 
					//other points in other polygons, to create an "snap" capability, only if "snap" is active:
					if(snapState)
					{
						for(int j = 0; j < MAX_SHAPES; j++)
						{
							if(shapeArray[j].type == POLY && j != workingOnPoly)	//If we found a polygon in the array, different that the one we are working with.!
							{
								//We go through the points and see if they collide with the one we are moving:
								for(int k = 0; k < shapeArray[j].poly->pN; k++)
								{
									ShapesUnion rectB = MakeBox(shapeArray[j].poly->pX[k] - uniDiv, 
										shapeArray[j].poly->pY[k] - uniDiv, 
										uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);
									if(CheckCollision(rect, rectB))	//If they do collide:
									{
										shapeArray[workingOnPoly].poly->pX[i] = shapeArray[j].poly->pX[k];
										shapeArray[workingOnPoly].poly->pY[i] = shapeArray[j].poly->pY[k];
										
										break;
									}									
										
									DestroyShape(rectB);
								}
							}
						}
					}
					
					break;
				}
				DestroyShape(rect);
			}
			savedX = gMouseStates.x;
			savedY = gMouseStates.y;
				
			RenderShape(rectMouse);
			DestroyShape(rectMouse);
		}
		totalTimeClick += dT;
	}
	//BUTTONS BEING PRESSED:
	else if(gKeyStates.left == SDL_PRESSED)
	{
		if(totalTimeClick == 0.0)
		{
			if(selectedState == POLYCREATE)
			{
				int16_t savedFirstPosX = shapeArray[workingOnPoly].poly->pX[0];
				int16_t savedFirstPosY = shapeArray[workingOnPoly].poly->pY[0];
				int8_t	savedPN = shapeArray[workingOnPoly].poly->pN;
				for(int i = 0; i < (savedPN - 1); i++)
				{
					shapeArray[workingOnPoly].poly->pX[i] = shapeArray[workingOnPoly].poly->pX[i + 1];
					shapeArray[workingOnPoly].poly->pY[i] = shapeArray[workingOnPoly].poly->pY[i + 1];
				}
				shapeArray[workingOnPoly].poly->pX[savedPN - 1] = savedFirstPosX;
				shapeArray[workingOnPoly].poly->pY[savedPN - 1] = savedFirstPosY;
			}
		}
		totalTimeClick += dT;
	}
	else if(gKeyStates.right == SDL_PRESSED)
	{
		if(totalTimeClick == 0.0)
		{
			if(selectedState == POLYCREATE)
			{
				int8_t	savedPN = shapeArray[workingOnPoly].poly->pN;
				int16_t savedLastPosX = shapeArray[workingOnPoly].poly->pX[savedPN - 1];
				int16_t savedLastPosY = shapeArray[workingOnPoly].poly->pY[savedPN - 1];
				for(int i = savedPN - 1; i > 0; i--)
				{
					shapeArray[workingOnPoly].poly->pX[i] = shapeArray[workingOnPoly].poly->pX[i - 1];
					shapeArray[workingOnPoly].poly->pY[i] = shapeArray[workingOnPoly].poly->pY[i - 1];
				}
				shapeArray[workingOnPoly].poly->pX[0] = savedLastPosX;
				shapeArray[workingOnPoly].poly->pY[0] = savedLastPosY;
			}
		}
		totalTimeClick += dT;
	}
	else
		totalTimeClick = 0.0;
	
	//////Render:
	if(selectedState == POLYCREATE)
	{
		
		//Render squares in points of polygons:
		int8_t divisor = 2;
		int16_t uniDiv = UNI / divisor;
		for(int i = 0; i < shapeArray[workingOnPoly].poly->pN; i++)
		{
			if(i == 0)
			{
				ShapesUnion circ = MakeCircle(shapeArray[workingOnPoly].poly->pX[i],
					shapeArray[workingOnPoly].poly->pY[i], uniDiv, 0, 0, 255, 255);
				RenderShape(circ);
				DestroyShape(circ);
			}
			else if(i == shapeArray[workingOnPoly].poly->pN - 1)
			{
				ShapesUnion circ = MakeCircle(shapeArray[workingOnPoly].poly->pX[i],
					shapeArray[workingOnPoly].poly->pY[i], uniDiv, 255, 0, 0, 255);
				RenderShape(circ);
				DestroyShape(circ);
			}
			else
			{
				ShapesUnion rect = MakeBox(shapeArray[workingOnPoly].poly->pX[i] - uniDiv, 
					shapeArray[workingOnPoly].poly->pY[i] - uniDiv, 
					uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);
				RenderShape(rect);
				DestroyShape(rect);
			}
		}
	}
	
	//Render Interface:
	RenderText(textShapes);
	vlineRGBA(gRenderer, UNI * 52, 0, SCREEN_HEIGHT, 0, 0, 0, 255);
	for(int i = 0; i < MAX_SHAPES; i++)	//Render Interface and everything else too:
	{
		if(shapeArray[i].type != NONE)	//If we found an occupied space in the array.
			RenderShape(shapeArray[i]);		
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
//~ #include "shapes.h"

//~ #define MAX_SHAPES 500

//~ #define UNI SCREEN_WIDTH / 64

//~ StatesEnum UpdateAndRenderObjectEditor(dT)
//~ {
	//~ static TTF_Font* textFont;
	//~ static Text* textShapes;
	
	//~ static SubStatesEnum subState = Load;
	
	//~ static ShapesUnion* shapeArray;
	
	
	//~ if(subState == Load)
	//~ {
		//~ subState = Normal;		
		//~ textFont = LoadFont("synchronizer_nbp.ttf", 16);
		//~ SDL_Color textColor = { 0, 0, 0, 255 };	
		//~ textShapes = MakeText("Shapes", textFont, textColor, UNI * 53, UNI);
		
		//~ shapeArray = calloc(MAX_SHAPES, sizeof(ShapesUnion));
		//~ //Every member of every element in shapeArray should be initialized to zero.
		
		//~ //Make interface:
		//~ shapeArray[0] = MakeBox(UNI * 54, UNI * 4, UNI * 2, UNI * 2, 127, 0, 0, 255);
		//~ shapeArray[1] = MakeCircle(UNI * 55, UNI * 8, UNI, 0, 127, 0, 255);
	//~ }
	
	//~ ////////Every Frame:
	//~ static ShapesEnum selectedShape = NONE;
	//~ if(gMouseStates.left == SDL_PRESSED)
	//~ {
		//~ int8_t divisor = 2;
		//~ int16_t uniDiv = UNI / divisor;
		//~ ShapesUnion rectMouse = MakeBox(gMouseStates.x - uniDiv, gMouseStates.y - uniDiv, 
			//~ uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);
		
		//~ if(CheckCollision(rectMouse, shapeArray[0]))
			//~ selectedShape = BOX;
		//~ else if (CheckCollision(rectMouse, shapeArray[1]))
			//~ selectedShape = CIRCLE;
		//~ else
		//~ {
			//~ for(int i = 0; i < MAX_SHAPES; i++)
			//~ {
				//~ if(shapeArray[i].type == NONE)		//If we found an empty space in the array.
				//~ {
					//~ if(selectedShape == BOX)
					//~ {
						//~ shapeArray[i] = MakeBox(gMouseStates.x, gMouseStates.y, 
							//~ uniDiv * divisor, uniDiv * divisor, 127, 0, 0, 255);
						//~ break;
					//~ }
				//~ }
			//~ }
		//~ }
		
		//~ RenderShape(rectMouse);
		//~ DestroyShape(rectMouse);
	//~ }
	
	//~ //Render Interface:
	//~ RenderText(textShapes);
	//~ vlineRGBA(gRenderer, UNI * 52, 0, SCREEN_HEIGHT, 0, 0, 0, 255);
	//~ for(int i = 0; i < MAX_SHAPES; i++)
	//~ {
		//~ if(shapeArray[i].type != NONE)
			//~ RenderShape(shapeArray[i]);		//If we found an occupied space in the array.
	//~ }
	
	//~ if(subState == Exit)
	//~ {
		//~ TTF_CloseFont(textFont);
		//~ DestroyText(textShapes);
		//~ for(int i = 0; i < MAX_SHAPES; i++)
		//~ {
			//~ if(shapeArray[i].type != NONE)
				//~ DestroyShape(shapeArray[i]);
		//~ }
		//~ free(shapeArray);
		
		//~ textFont = NULL;
		//~ textShapes = NULL;
		//~ shapeArray = NULL;
		
		//~ subState = Load;		
	//~ }
	
	//~ return ObjectEditor;
//~ }


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
