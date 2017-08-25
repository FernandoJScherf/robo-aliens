//Using SDL, SDL_image, standard IO, and strings

#include "rob.h"

void closeGame()
{
	//TO DO: Free FONT. Maybe make sure to execute the Exit part of the code of the active state(s)!
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	//TO DO: Quit FONT subsystem.
}

int main( int argc, char* args[] )
{
	//INITIALIZE-----------------------------------------------------------------------------------:
	//Initialize SDL:
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL_Init failed! SDL_GetError(): %s\n", SDL_GetError());
		return 1;
	}
	//Set texture filtering to linear:
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )	
	{
		printf( "Warning: Linear texture filtering not enabled!" );
	}	//By default nearest pixel sampling is used, but just to make sure, lets leave this here.
	//Create Window:
	gWindow = SDL_CreateWindow( "Testing vSync!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if(gWindow == NULL)
	{
		printf("SDL_CreateWindow failed! SDL_GetError(): %s\n", SDL_GetError());
		return 2;
	}
	//Create renderer for window:
	//hardware acceleration + synchronized with the refresh rate
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL)
	{
		printf("SDL_CreateRenderer failed! SDL_GetError(): %s\n", SDL_GetError());
		return 3;
	}
	 //Initialize SDL_ttf
	if(TTF_Init() == -1 )
	{
		printf( "TTF_Init() failed! TTF_GetError(): %s\n", TTF_GetError());
		return 4;
	}
	
	////LOAD MEDIA-----------------------------------------------------------------------------------:
	//Open the font
	TTF_Font* textFont = LoadFont("synchronizer_nbp.ttf", 16);
	//Set text color:
	SDL_Color textColor = { 0, 0, 0, 255 };
	
	/////////		
		
	uint8_t refreshDiv = 1;
	uint8_t refreshRate = 60;	//Default 60 Hz.
	//Is vsync on?:
	SDL_RendererInfo renderInfo;
	if(SDL_GetRendererInfo(gRenderer, &renderInfo) < 0)
	{
		printf("SDL_GetRendererInfo failed! SDL_GetError(): %s\n", SDL_GetError());
		return 6;
	}
	uint8_t vsyncOn = renderInfo.flags & SDL_RENDERER_PRESENTVSYNC; //vsyncOn will be != 0 if vsync is on.
	if(vsyncOn)	//If on, set the refreshRate variable to the refresh rate of the user's screen. Then get refresh divider.
	{
		printf("VSYNC IS ON!\n");
		//Declare display mode structure to be filled in:
		SDL_DisplayMode currentDisplayInfo;
		//Get current display mode of display 0 (ASSUMING THERE IS ONLY ONE DISPLAY):
		if(SDL_GetCurrentDisplayMode(0, &currentDisplayInfo) != 0)
		{
			printf("SDL_GetCurrentDisplayMode failed! SDL_GetError(): %s\n", SDL_GetError());
			return 7;
		}
		//Calculate refresh rate divisor:
		refreshRate = currentDisplayInfo.refresh_rate;
		while((refreshRate / refreshDiv) > 100)
			refreshDiv++;	
	}
	double dT = (double) refreshDiv / refreshRate;
	printf("Refresh Rate = %i, Refresh Divider = %i, delta time = %f\n", refreshRate, refreshDiv, dT);
	
	//----------Game Loop----------:	
	uint8_t goOn = 1;
	uint64_t tAnter = 0, tActual;
	double tFrame;
	uint64_t tAnterReal = 0, tActualReal;
	float tFrameReal, tFrameRealAcum = 0;
	uint8_t tFrameRealCount = 0;
	float tFrameRealProm;
	while(goOn)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
				goOn = 0;
			else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_LEFT:
						gKeyStates.left = e.key.state;
						break;
					case SDLK_RIGHT:
						gKeyStates.right = e.key.state;
						break;
					case SDLK_SPACE:
						gKeyStates.jump = e.key.state;
						break;
				}
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				switch(e.button.button)
				{
					case SDL_BUTTON_LEFT:
						gMouseStates.left = e.button.state;
						break;
					case SDL_BUTTON_RIGHT:
						gMouseStates.right = e.button.state;
						break;
					case SDL_BUTTON_MIDDLE:
						gMouseStates.middle = e.button.state;
						break;
				}
			}
			else if(e.type == SDL_MOUSEMOTION)
			{
				gMouseStates.x = e.motion.x;
				gMouseStates.y = e.motion.y;
			}
		}
		
		//~ if(gMouseStates.left == SDL_PRESSED)
			//~ printf("Pressed %i %i\n", gMouseStates.x, gMouseStates.y);
		//~ else
			//~ printf("Not Pressed\n");
		
		//CALCULATE REAL TIME BETWEEN FRAMES AND FRAMERATE:	
		tActualReal = SDL_GetPerformanceCounter();
		tFrameReal = (double)(tActualReal - tAnterReal) / SDL_GetPerformanceFrequency();
		tAnterReal = tActualReal;
		tFrameRealAcum += tFrameReal;
		tFrameRealCount++;
		if(tFrameRealCount > 19)
		{
			tFrameRealProm = tFrameRealAcum / tFrameRealCount;
			//printf("tFrameRealCount = %i, tFrameRealProm = %f\n", tFrameRealCount, tFrameRealProm);
			//printf("FrameRate = %f\n", 1/tFrameRealProm);
			tFrameRealAcum = 0;	tFrameRealCount = 0;
		}
		
		//Clear Screen:
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); //Reset the render draw color every frame.
		SDL_RenderClear(gRenderer);
		
		//Call the update and render function where THE MAGIC HAPPENS:
		UpdateAndRender(dT);
		
		//Show framerate on screen:
		char fpsStr[20];
		sprintf(fpsStr, "%.6f", 1/tFrameRealProm);
		//RenderText(MakeText(fpsStr, textFont, textColor, 1, SCREEN_HEIGHT - TTF_FontHeight(textFont) - 1));
		Text* fpsText = MakeText(fpsStr, textFont, textColor, 10, 10);
		RenderText(fpsText);
		DestroyText(fpsText);
		
		/////////TO DO: Show fps depending on some conditional.
		
		if(	!vsyncOn)		//If Vsync is off, cap framerate!
		{
			tActual = SDL_GetPerformanceCounter();
			tFrame = (double)(tActual - tAnter) / SDL_GetPerformanceFrequency();
			while(tFrame < dT)
			{
				SDL_Delay((dT - tFrame) * 1000);	//GOT TO BE IN MILISECONDS!
				//I could consider to not use SDL_Delay to have a framerate closer to 60 (With this, we have ~59.8
				tActual = SDL_GetPerformanceCounter();
				tFrame = (double)(tActual - tAnter) / SDL_GetPerformanceFrequency();
			}
			tAnter = tActual;
		}
		
		for(uint8_t i = 0; i < refreshDiv; i++)	//If vsync is on, but a refresh rate TOO DAMN HIGH FOR SOME REASON
		{										//I repeat frames in 1, 2, 3, etc, to "simulate" a lower refresh rate.
			SDL_RenderPresent(gRenderer);		//So the time between frames always really is dT = (double) refreshDiv / refreshRate;	
		}
													
	}
	
	atexit(closeGame);
}

	//ALL REFERENT TO GAME LOOP--------------------------------------------------------------------:
	//This is designed to work as follow, taking into consideration different circumstances:
	//I will call "refreshrate" the number of times the screen is updated by the graphics card
	//(Usually 60 Hz) in a second and "framerate" the actual
	//numbers of times the game renders a frame in a second.
	//>VSync On: Ideally, this is always the case. The game and code must be designed to achieve
	//framerate == refreshrate constantly. refreshrate must always be the upper limit of framerate,
	//which is easily achieved using vsync. Only during rare occasions (Like system events on iOS)
	//(Or when the game runs on REALLY slow systems)
	//we should get framerate < refreshrate, and that should cause a slowdown. This means, the game
	//continue playing, but with lower (Or much lower) framerate. There must be no missing frames.
	//The player must see always every frame, to avoid unfair situations, like making a jump
	//during a moment of low framerate and not beeing able to see where the controlled character is 
	//going.
	//>VsyncOff: This probably will only happen if the player deactivates vsync for his operating
	//system from the options of his graphic driver. The framerate will be kept at a maximum value
	//(Probably 60 Hz) by other means. Deactivating the vsyn will probably cause screen tearing,
	//but well, thats the choice of the player. Also in case of a framerate drop, we will have
	//a slowdown, no missing frames.
