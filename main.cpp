#include <SDL.h>
#include <iostream>
#include "Manager.h"
#include "App.h"
#include "Deck.h"

App *app = nullptr;
const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;

/*
BUGS:
	Delay on start button
	Fix A of Hearts
	Randomly cards 2 and 3 went bigger when player1 empt his deck
*/


int main(int argc, char **argv)
{
	app = new App();

	bool init = app->init("WarWithCards",
						  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	

	if (!init)
	{
		return -1;
	}

	app->ttf_init();
	while (app->isRunning())
	{
		app->handleEvents();
		app->render();
	}

	delete app;

	return 0;
}