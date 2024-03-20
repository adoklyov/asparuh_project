#include <SDL.h>
#include <iostream>
#include "Manager.h"
#include "App.h"
#include "Deck.h"
#include "Player.h"

int main(int argc, char **argv)
{
    const unsigned WINDOW_WIDTH = 800;
    const unsigned WINDOW_HEIGHT = 600;

    Deck deck;
    Player player1, player2, player3;

    std::vector<Player> players{player1, player2, player3};

    Manager manager(player1, player2, player3);

    App *app = new App(players, manager);
    bool initSuccess = app->init("WarWithCards",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (!initSuccess) {
        std::cerr << "Failed to initialize the app\n";
        delete app;
        return -1; 
    }

    if (!app->ttf_init()) {
        std::cerr << "Failed to initialize TTF\n";
        delete app;
        return -1; 
    }

    deck.shuffle();
    deck.riffleShuffle(); 
    // player1.dealCards(deck);
    // player2.dealCards(deck);
    // player3.dealCards(deck);


    app->loadTextureOnDeck(deck);


    while (app->isRunning()) {
        app->handleEvents(deck, manager);
        app->update(); 
        app->render(); 
    }

    app->DestroySDL();
    delete app;

    return 0; 
}
