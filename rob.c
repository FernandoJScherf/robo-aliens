#include "rob.h"
#include "UpdateAndRenderTest.h"

//The window we'll be rendering to:
SDL_Window* gWindow = NULL;
//The window renderer:
SDL_Renderer* gRenderer = NULL;

SDL_Texture* LoadTexture(char* file)
{
	//Load image in file:
	SDL_Surface* loadedSurface = IMG_Load(file);
	if(loadedSurface == NULL)
		printf("IMG_Load failed! Unable to load image %s! IMG_GetError(): %s\n", file, IMG_GetError());
	//The final texture:
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface); //Will be null if texture couldn't be created.
	//Get rid of the surface (That we wont use anymore):
	SDL_FreeSurface(loadedSurface);
	if(texture == NULL)
		printf("SDL_CreateTextureFromSurface failed! Unable to create texture from %s! SDL_GetError(): %s\n", file, SDL_GetError());
	return texture;
}	//Must be paired with a SDL_DestroyTexture(texture) always.

SpriteAnimation* LoadAnimation(SDL_Texture* texture, int spriteWidth, int spriteHeight,
								int frameFirst, int frameQuant)
{
	int textureWidth;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, NULL);
	
	SpriteAnimation* animation = malloc(sizeof(SpriteAnimation));
	
	animation->texture 			= texture;
	animation->textureWidth 	= textureWidth;
	animation->spriteWidth 		= spriteHeight;
	animation->spriteHeight 	= spriteWidth;
	animation->frameFirst 		= frameFirst;
	animation->frameQuant 		= frameQuant;
	
	return animation;
}	//Must be paired with a free(animation) always.

Entity* MakeEntity(SpriteAnimation* animation, int8_t framesPerSecond, float x, float y, int xVel, int yVel/*, SDL_Rect colBox*/)
{
	Entity* entity = malloc(sizeof(Entity));
	
	entity->state 				= 0;
	entity->animation 			= animation;
	entity->animFrameNow 		= 0;
	entity->animTimeAcum 		= 0;
	entity->animFramesPerSecond = framesPerSecond;
	entity->x					= x;
	entity->y					= y;
	entity->xVel				= xVel;
	entity->yVel 				= yVel;
	return entity;
}	//Must be paired with a free(entity) always.

void RenderEntity(Entity* entity, double dT)
{
	//TO DO: Do I really need double precision instead of just float for dT and company?
	SpriteAnimation Lanimation = *(entity->animation);
	
	int xFrameInTexture = Lanimation.spriteWidth * (Lanimation.frameFirst + entity->animFrameNow);		
	int yFrameInTexture = 0;
	for(int i = 0; i < xFrameInTexture / Lanimation.textureWidth; i++)
		yFrameInTexture += Lanimation.spriteHeight;
	while(xFrameInTexture >= Lanimation.textureWidth)
		xFrameInTexture -= Lanimation.textureWidth;

	SDL_Rect srcrect = {xFrameInTexture, yFrameInTexture, Lanimation.spriteWidth, Lanimation.spriteHeight};
	SDL_Rect dstrect = {entity->x, entity->y, Lanimation.spriteWidth, Lanimation.spriteHeight};
	
	SDL_RenderCopy(gRenderer, Lanimation.texture, &srcrect, &dstrect);
	
	//Go to next frame o go back to the first one?:
	int16_t LframeLast = (double)entity->animFramesPerSecond * entity->animTimeAcum; // LtimeAcum / (1 / framespersecond)
	entity->animTimeAcum += dT;
	int16_t LframeNow = (double)entity->animFramesPerSecond * entity->animTimeAcum;
	if(LframeLast != LframeNow)
		entity->animFrameNow++;
	
	if(entity->animFrameNow == Lanimation.frameQuant)
		entity->animFrameNow = 0;
}

TTF_Font* LoadFont(char* file, int8_t size)
{
	TTF_Font* font = TTF_OpenFont(file, size);
	if(font == NULL )
		printf( "TTF_OpenFont failed! TTF_GetError(): %s\n", TTF_GetError());
	return font;	//Will return NULL if font wasn't correctly loaded.
}

Text* MakeText(char* toWrite, TTF_Font* font, SDL_Color color, float x, float y)
{
	Text* text = malloc(sizeof(Text));
	//Make text surface:
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, toWrite, color);
	if(textSurface == NULL)
		printf( "TTF_RenderText_Solid failed! TTF_GetError(): %s\n", TTF_GetError());
	
	//Create texture from surface:
	text->texture = SDL_CreateTextureFromSurface(gRenderer, textSurface); //Will be null if texture couldn't be created.
	
	if(text->texture == NULL)
		printf( "SDL_CreateTextureFromSurface failed! TTF_GetError(): %s\n", TTF_GetError());
	
	text->textRect.x = x;
	text->textRect.y = y;
	text->textRect.w = textSurface->w;
	text->textRect.h = textSurface->h;
	
	//Get rid of the surface (That we wont use anymore):
	SDL_FreeSurface(textSurface);	
	
	return text;
}	//MUST BE ALWAYS PAIRED WITH DestroyText(text)

void RenderText(Text* text)
{
	SDL_RenderCopy(gRenderer, text->texture, NULL, &text->textRect);
}

void DestroyText(Text* text)
{
	SDL_DestroyTexture(text->texture);
	free(text);
}

void UpdateAndRender(double dT)
{
	static StatesEnum state	= Test;

	switch(state)
	{
		case Test:
			state = UpdateAndRenderTest(dT);
			break;
	}
}


