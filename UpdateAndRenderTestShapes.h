#include "rob.h"

#define MAX_ENTITIES 10000

typedef struct
{
	float				x1;
	float				y1;
	int 				x2;
	int 				y2;
	int 				x3;	//for the
	int 				y3;	//triangle
	int8_t				r;
	int8_t				g;
	int8_t				b;
	int 				xVel;
	int 				yVel;
} ShapeRect;

StatesEnum UpdateAndRenderTestShapes(double dT)
{
	static SubStatesEnum subState = Load;
	static ShapeRect rectArray[MAX_ENTITIES];
	static int rectCount = 0;
	//Open the font
	static TTF_Font* textFont;
	//Set text color:
	SDL_Color textColor = { 0, 0, 0, 255 };	
	
	if(subState == Load)
	{
		subState = Normal;		
		textFont = LoadFont("synchronizer_nbp.ttf", 16);
		
		srand(time(NULL)); //Should only be called once. Give seed to semi-random umber generator.
	}
	
	
	////////Every frame:
	if(gKeyStates.jump == SDL_PRESSED)
	{		
		for(int i = 0; i < 10; i++)
		{
			rectArray[rectCount].x1		= rand() % SCREEN_WIDTH;
			rectArray[rectCount].y1		= rand() % SCREEN_HEIGHT;
			rectArray[rectCount].x2 	= rectArray[rectCount].x1 + rand() % 50 + 5;
			rectArray[rectCount].y2		= rectArray[rectCount].y1 + rand() % 50 + 5;
			
			rectArray[rectCount].x3 	= rectArray[rectCount].x1 + rand() % 50 + 5;
			rectArray[rectCount].y3		= rectArray[rectCount].y1 + rand() % 50 + 5;
			
			rectArray[rectCount].r		= rand() % 255;
			rectArray[rectCount].g		= rand() % 255;
			rectArray[rectCount].b		= rand() % 255;
			rectArray[rectCount].xVel	= 0;
			rectArray[rectCount].yVel	= 0;
			rectCount++;
			if(rectCount == MAX_ENTITIES)
				rectCount = 0;
		}
	}
	//if(gKeyStates.right == 1)	This must change to the next shape! 
		
	for(int i = 0; i < rectCount; i++)
		filledTrigonRGBA(gRenderer, rectArray[i].x1, rectArray[i].y1, 
						rectArray[i].x2, rectArray[i].y2, 
						rectArray[i].x3, rectArray[i].y3, rectArray[i].r, rectArray[i].g, rectArray[i].b, 255);
		//~ filledCircleRGBA (gRenderer, rectArray[i].x1, rectArray[i].y1, 25, 
					//~ rectArray[i].r, rectArray[i].g, rectArray[i].b, 255);
		//~ boxRGBA(gRenderer, rectArray[i].x1, rectArray[i].y1, rectArray[i].x2, rectArray[i].y2, 
								//~ rectArray[i].r, rectArray[i].g, rectArray[i].b, 255);
		//~ rectangleRGBA(gRenderer, rectArray[i].x1, rectArray[i].y1, rectArray[i].x2, rectArray[i].y2, 
								//~ 
								
	//Show actual number of entities on screen.
	char str[20];
	sprintf(str, "%i", rectCount);
	Text* text = MakeText(str, textFont, textColor, 10, SCREEN_HEIGHT - 20);
	RenderText(text);
	DestroyText(text);
	
	if(subState == Exit)
	{
		TTF_CloseFont(textFont);
	}
	
	return TestShapes;
}
