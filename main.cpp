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
	Deck deck;
	Player player1, player2, player3;
	std::vector<Player> players {player1, player2, player3};

	app = new App(players);

	bool init = app->init("WarWithCards",
						  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, deck, players);
	
	Manager manager(players[0], players[1], players[2]); // The error: It passes Players without cards


	if (!init)
	{
		return -1;
	}

	app->ttf_init();
	while (app->isRunning())
	{
		app->handleEvents(deck, manager, players);
		app->update();
		app->render(players);
	}
	app->DestroySDL();

	delete app;

	return 0;
}