#include <SDL.h>
#include <iostream>
#include "App.h"
#include "Deck.h"

App *app = nullptr;
const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;


//BUGS:
//Проблема с ERROR-ите за сега е решен като просто добавих this-> пред war понеже компилатора е прост явно
//При умирането във война картите на плеъра се трият без тази, които са му в мини дека.
//В последния рунд, когато излезе война(споледни бяха 1 и 2) картата на умрелия преди това се появи, а картите от мини дека на победителя остават на екрана 
//Да се фиксне в стейт OVER, да не брои повече от една победа
//memory leak

//решения: 
//1.Да се изчака за следващия рунд и тогава да се изтрият, така ще може по лесно да се проследи играта 
//2.Да се коригира така че да се трие и мини дека
//3.Като умре играч да се махне просто от рендърването и да не се рендърва нищо от него(имай предвид, че мини дека е в throwcards)
//Разширения:
//Да се изписва на екрана кой е взел картите от война за по лесно четима игра, потенциално с нов стейт WINROUND
//Да се измести локацията на надписа War, за да не е покрит от мини дека при дълги игри
//Музика/Звуци

int main(int argc, char **argv)
{
	app = new App();

	bool init = app->init("WarWithCards",
						  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

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