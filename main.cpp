#include <SDL.h>
#include <iostream>
#include "App.h"
#include "Deck.h"

App *app = nullptr;
const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;


//BUGS:
//При умирането във война картите на плеъра се трият без тeзи, които са му в мини дека.
//В последния рунд, когато излезе война(споледни бяха 1 и 2) картата на умрелия преди това се появи, а картите от мини дека на победителя остават на екрана 
//Да се фиксне в стейт OVER, да не брои повече от една победа
//memory leak- done
//След война стейта на война стои докато не се цъкнат всички Deal - done
//При използване на бутоните Deal, масата трие само първите карти без мини картите - done
//Проблема с ERROR-ите за сега е решен като просто добавих this-> пред war понеже компилатора е прост явно - done
//Когато някой вече няма карти, за да се продължи с рунда трябва се цъкне бутона Deal и на неиграещия, същото е за войната - done

//решения: 
//1.Да се изчака за следващия рунд и тогава да се изтрият, така ще може по лесно да се проследи играта 
//2.Да се коригира така че да се трие и мини дека 
//3.Като умре играч да се махне просто от рендърването и да не се рендърва нищо от него(имай предвид, че мини дека е в throwcards)
//Разширения:
//Музика/Звуци
//Да се измести локацията на надписа War, за да не е покрит от мини дека при дълги игри - done
//Да се изписва на екрана кой е взел картите от война за по лесно четима игра, потенциално с нов стейт WINROUND - done


int main(int argc, char **argv)
{
	const int FPS = 144;
	const int frameDelay = 1000/FPS;

	Uint32 frameStart;
	int frameTime;

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
		frameStart = SDL_GetTicks();

		app->handleEvents();
		app->render();
		app->update();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
		
	}

	delete app;

    return 0;
}
