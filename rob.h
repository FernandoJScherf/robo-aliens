#ifndef ROB_H
#define ROB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_ttf.h>

//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

//The window we'll be rendering to:
SDL_Window* gWindow;
//The window renderer:
SDL_Renderer* gRenderer;

typedef struct
{
	SDL_Texture* 		texture;
	int					textureWidth;
	int 				spriteWidth;
	int					spriteHeight;
	int					frameFirst;
	int					frameQuant;
} SpriteAnimation;

typedef struct
{
	int8_t 				state;
	SpriteAnimation*	animation;
	int16_t				animFrameNow;
	double 				animTimeAcum;
	int8_t				animFramesPerSecond;
	float				x;
	float				y;
	int 				xVel;
	int 				yVel;
	//SDL_Rect			colBox;
} Entity;

typedef struct
{
	SDL_Texture*		texture;
	SDL_Rect			textRect;
}Text;

typedef enum
{
	Test,
	Menu
}StatesEnum;

typedef enum
{
	Load,
	Normal,
	Exit
}SubStatesEnum;

struct KeyStates
{
	int8_t left;
	int8_t right;
	int8_t jump;
}gKeyStates; 			//A GLOBAL STRUCTURE THAT HOLDS IF A PARTICULAR KEY HAS BEEN PRESSED!

SDL_Texture* LoadTexture(char* file);	//Must be paired with a SDL_DestroyTexture(texture) always.

SpriteAnimation* LoadAnimation(SDL_Texture* texture, int spriteWidth, int spriteHeight,
								int frameFirst, int frameQuant);
								//Must be paired with a free(animation) always.

//TO DO: OPTIMIZE int TO int8_t IF POSSIBLE!

Entity* MakeEntity(SpriteAnimation* animation, int8_t framesPerSecond, float x, float y, int xVel, int yVel/*, SDL_Rect colBox*/);	//collision box y RELATIVE to the position of the entity.
//Must be paired with a free(entity) always.

void RenderEntity(Entity* entity, double dT);

TTF_Font* LoadFont(char* file, int8_t size);

Text* MakeText(char* toWrite, TTF_Font* font, SDL_Color color, float x, float y);
//MUST BE ALWAYS PAIRED WITH DestroyText(text)

void RenderText(Text* text);

void DestroyText(Text* text);

void UpdateAndRender(double dT);

#endif
