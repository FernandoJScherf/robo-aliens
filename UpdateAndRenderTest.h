#include "rob.h"

#define MAX_ENTITIES 125

StatesEnum UpdateAndRenderTest(double dT)
{
	static SubStatesEnum subState = Load;
	
	static SDL_Texture* animTexture;
	static SpriteAnimation* anim;
	static SpriteEntity** entArray;
	
	if(subState == Load)
	{
		subState = Normal;
		
		//Here we could have an small "loading" render if this takes to many frames.
		
		srand(time(NULL)); //Should only be called once. Give seed to semi-random umber generator.
		
		animTexture = LoadTexture("Face.pbm");
		anim = LoadAnimation(animTexture, 48, 48, 0, 30);
		
		entArray = calloc(MAX_ENTITIES, sizeof(SpriteEntity*));	//Allocate the space needed and initialize all to zero.
	}
	//Every frame:
	if(gKeyStates.jump == SDL_PRESSED)
	{
		//Add new (semi-randomized) entity to the array:
		int lx = rand() % SCREEN_WIDTH;		//Generate "random" starting position.
		int ly = rand() % SCREEN_HEIGHT;	//It's common practice to use the % operator to get a different range.
		int lxVel = rand() % 30;
		int lyVel = rand() % 30;
		
		for(int i = 0; i < MAX_ENTITIES; i++)
		{
			if(entArray[i] == 0)	//If we found an empty space in the array.
			{
				entArray[i] = MakeSpriteEntity(anim, rand() % 15, lx, ly, lxVel, lyVel);
				break;
			}
		}
	}
	
	//For all entities, update position and render.
	for(int i = 0; i < MAX_ENTITIES; i++)
	{		
		if(entArray[i] != 0)	//If we found an occupied space in the array.
		{
			entArray[i]->x = entArray[i]->x + entArray[i]->xVel * dT;
			entArray[i]->y = entArray[i]->y + entArray[i]->yVel * dT;
			if(entArray[i]->x < 0)
			{
				entArray[i]->xVel *= -1;
				entArray[i]->x = 0;
			}
			else if(entArray[i]->x > (SCREEN_WIDTH - entArray[i]->animation->spriteWidth))
			{
				entArray[i]->xVel *= -1;
				entArray[i]->x = SCREEN_WIDTH - entArray[i]->animation->spriteWidth;
			}
			//////
			if(entArray[i]->y < 0)
			{
				entArray[i]->yVel *= -1;
				entArray[i]->y = 0;
			}
			else if(entArray[i]->y > (SCREEN_HEIGHT - entArray[i]->animation->spriteHeight))
			{
				entArray[i]->yVel *= -1;
				entArray[i]->y = SCREEN_HEIGHT - entArray[i]->animation->spriteHeight;
			}
			
			RenderSpriteEntity(entArray[i], dT);
			
		}
	}
	
	hlineRGBA(gRenderer, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0 , 0 , 0, 255);
	
	if(subState == Exit)
	{
		SDL_DestroyTexture(animTexture);
		free(anim); 		anim = NULL;
		free(entArray);		entArray = NULL;
	}
	
	return Test;
}

//~ StatesEnum UpdateAndRenderTest(double dT)
//~ {	
	//~ static SubStatesEnum subState = Load;
	//~ static SpriteAnimation* caveAnim;
	
	//~ static TTF_Font* textFont;
	//~ static SDL_Color textColor = { 0, 0, 0, 255 };
	//~ static Text helloWorldText;
	
	//~ static SpriteEntity* ent = NULL;
	//~ static int entQuantNow = 0;
	
	//~ if(subState == Load)
	//~ {
		//~ srand(time(NULL)); //Should only be called once. Give seed to semi-random umber generator.
		
		//~ caveAnim = malloc(sizeof(SpriteAnimation));
		//~ *caveAnim = LoadAnimation("Face.pbm", 48, 48, 6, 7, 4);
		
		//~ ent = malloc(MAX_ENTITIES * sizeof(SpriteEntity));
		//~ //Open the font
		//~ textFont = LoadFont("synchronizer_nbp.ttf", 24);
				
		//~ subState = Normal;
	//~ }
	//~ ////////////////////////Every frame:
	
	//~ if(gKeyStates.jump == 1)
	//~ {
		//~ int lx = rand() % SCREEN_WIDTH;		//Generate "random" starting position.
		//~ int ly = rand() % SCREEN_HEIGHT;	//It's common practice to use the % operator to get a different range.
		//~ int lxVel = rand() % 30;
		//~ int lyVel = rand() % 30;
		//~ //Add new entity to the list:
		//~ ent[entQuantNow] = MakeSpriteEntity(caveAnim, lx, ly, lxVel, lyVel);
		//~ entQuantNow++;
		//~ //If we surpass the limit, start overwriting.
		//~ if(entQuantNow == MAX_ENTITIES)
			//~ entQuantNow = 0;
	//~ }
	
	//~ //Move entities around: 
	//~ for(int i = 0; i < entQuantNow; i++)	
	//~ {
		//~ ent[i].x = ent[i].x + ent[i].xVel * dT;
		//~ ent[i].y = ent[i].y + ent[i].yVel * dT;
		
		//~ if(ent[i].x < 0)
		//~ {
			//~ ent[i].xVel *= -1;
			//~ ent[i].x = 0;
		//~ }
		//~ else if(ent[i].x > (SCREEN_WIDTH - ent[i].animation->spriteWidth))
		//~ {
			//~ ent[i].xVel *= -1;
			//~ ent[i].x = SCREEN_WIDTH - ent[i].animation->spriteWidth;
		//~ }
			//~ ////
		//~ if(ent[i].y < 0)
		//~ {
			//~ ent[i].yVel *= -1;
			//~ ent[i].y = 0;
		//~ }
		//~ else if(ent[i].y > (SCREEN_HEIGHT - ent[i].animation->spriteHeight))
		//~ {
			//~ ent[i].yVel *= -1;
			//~ ent[i].y = SCREEN_HEIGHT - ent[i].animation->spriteHeight;
		//~ }
		
		//~ RenderSpriteEntity(ent+i, dT);
	//~ }
		
	//~ //Show actual number of entities on screen.
	//~ char quantStr[20];
	//~ sprintf(quantStr, "%i", entQuantNow);
	//~ helloWorldText = MakeText(quantStr, textFont, textColor, 10, 10);
	//~ RenderText(&helloWorldText);
	//~ ////////////////////////////////////
	
	//~ if(subState == Exit)
	//~ {
		//~ free(ent);	//Free memory allocated for all entity structs.
		
		//~ //Free loaded texture:
		//~ SDL_DestroyTexture(caveAnim->texture);
		
		//~ free(caveAnim);	//Free memory allocated for animation structs.
		
		//~ SDL_DestroyTexture(helloWorldText.texture);
		//~ TTF_CloseFont(textFont);
		
		//~ subState = Load;
		//~ //TO DO: Probably it would be a good idea to manage what happens on the quit event from every state, so it can clean up
		//~ //every texture before quiting sdl. But probably this is not really necessary.
	//~ }
	//~ return Test;
//~ }

//~ StatesEnum UpdateAndRenderTest(double dT)
//~ {	
	//~ static SubStatesEnum subState = Load;
	//~ static SpriteAnimation caveAnim;
	
	//~ static TTF_Font* textFont;
	//~ static SDL_Color textColor = { 0, 0, 0, 255 };
	//~ static Text helloWorldText;
	
	//~ static SpriteEntity ent[MAX_ENTITIES];
	//~ static int8_t entQuantNow = 0;
	
	//~ if(subState == Load)
	//~ {
		//~ srand(time(NULL)); //Should only be called once. Give seed to semi-random umber generator.
		
		//~ caveAnim = LoadAnimation("Face.pbm", 48, 48, 6, 7, 4);
		
		//~ //Open the font
		//~ textFont = LoadFont("synchronizer_nbp.ttf", 24);
				
		//~ subState = Normal;
	//~ }
	//~ ////////////////////////Every frame:
	
	//~ if(gKeyStates.jump == 1)
	//~ {
		//~ int lx = rand() % SCREEN_WIDTH;		//Generate "random" starting position.
		//~ int ly = rand() % SCREEN_HEIGHT;	//It's common practice to use the % operator to get a different range.
		//~ int lxVel = rand() % 30;
		//~ int lyVel = rand() % 30;
		//~ //Add new entity to the list:
		//~ ent[entQuantNow] = MakeSpriteEntity(&caveAnim, lx, ly, lxVel, lyVel);
		//~ entQuantNow++;
		//~ //If we surpass the limit, start overwriting.
		//~ if(entQuantNow == MAX_ENTITIES)
			//~ entQuantNow = 0;
	//~ }
	
	//~ //Move entities around: 
	//~ for(int i = 0; i < entQuantNow; i++)	
	//~ {
		//~ ent[i].x = ent[i].x + ent[i].xVel * dT;
		//~ ent[i].y = ent[i].y + ent[i].yVel * dT;
		
		//~ if(ent[i].x < 0)
		//~ {
			//~ ent[i].xVel *= -1;
			//~ ent[i].x = 0;
		//~ }
		//~ else if(ent[i].x > (SCREEN_WIDTH - ent[i].animation->spriteWidth))
		//~ {
			//~ ent[i].xVel *= -1;
			//~ ent[i].x = SCREEN_WIDTH - ent[i].animation->spriteWidth;
		//~ }
			//~ ////
		//~ if(ent[i].y < 0)
		//~ {
			//~ ent[i].yVel *= -1;
			//~ ent[i].y = 0;
		//~ }
		//~ else if(ent[i].y > (SCREEN_HEIGHT - ent[i].animation->spriteHeight))
		//~ {
			//~ ent[i].yVel *= -1;
			//~ ent[i].y = SCREEN_HEIGHT - ent[i].animation->spriteHeight;
		//~ }
		
		//~ RenderSpriteEntity(ent+i, dT);
	//~ }
		
	//~ //Show actual number of entities on screen.
	//~ char quantStr[20];
	//~ sprintf(quantStr, "%i", entQuantNow);
	//~ helloWorldText = MakeText(quantStr, textFont, textColor, 10, 10);
	//~ RenderText(&helloWorldText);
	//~ ////////////////////////////////////
	
	//~ if(subState == Exit)
	//~ {
		//~ //Free loaded texture:
		//~ SDL_DestroyTexture(caveAnim.texture);
		//~ caveAnim.texture = NULL;
		//~ //TO DO: Probably it would be a good idea to manage what happens on the quit event from every state, so it can clean up
		//~ //every texture before quiting sdl. But probably this is not really necessary.
	//~ }
	//~ return Test;
//~ }

//StatesEnum UpdateAndRenderTest(double dT)
//{
	//static SubStatesEnum subState = Load;
	//static SpriteAnimation *pCaveAnim;
	//static SpriteEntity *pCaveEnt;
	
	//if(subState == Load)
	//{
		//pCaveAnim = malloc(sizeof(SpriteAnimation));
		//*pCaveAnim = LoadAnimation("Face.pbm", 48, 48, 0, 5, 2);
		
		//pCaveEnt = malloc(sizeof(SpriteEntity));
		//SDL_Rect caveRect = {0, 0, pCaveAnim->spriteWidth, pCaveAnim->spriteHeight};
		//*pCaveEnt = MakeSpriteEntity(pCaveAnim, SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, caveRect);
		
		//subState = Normal;	
	//}
	
	//////Every frame:
	//static int xVel = 300, yVel = 300;
	//pCaveEnt->x = pCaveEnt->x + xVel * dT;
	//pCaveEnt->y = pCaveEnt->y + yVel * dT;
	
	//if(pCaveEnt->x < 0)
	//{
		//xVel *= -1;
		//pCaveEnt->x = 0;
	//}
	//else if(pCaveEnt->x > (SCREEN_WIDTH - pCaveEnt->animation->spriteWidth))
	//{
		//xVel *= -1;
		//pCaveEnt->x = SCREEN_WIDTH - pCaveEnt->animation->spriteWidth;
	//}
		
	//if(pCaveEnt->y < 0)
	//{
		//yVel *= -1;
		//pCaveEnt->y = 0;
	//}
	//else if(pCaveEnt->y > (SCREEN_HEIGHT - pCaveEnt->animation->spriteHeight))
	//{
		//yVel *= -1;
		//pCaveEnt->y = SCREEN_HEIGHT - pCaveEnt->animation->spriteHeight;
	//}
	
	//RenderSpriteEntity(pCaveEnt, dT);
	
	//if(subState == Exit)
	//{
		////Free loaded texture:
		//SDL_DestroyTexture(pCaveAnim->texture);

		//free(pCaveAnim);
		//pCaveAnim = NULL;
		//free(pCaveEnt);
		//pCaveEnt = NULL;
	//}
	
	//return Test;	//This will change to some variable that contains Test if next frame continues or some other state.
//}

