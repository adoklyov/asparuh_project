#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Player.h"
#include "Manager.h"
#include <vector>
enum GameState
{
	START,
	PLAYING,
	WAR,
	OVER
};

class App
{
	friend bool operator>(const Card& card1, const Card& card2);
	friend bool operator<(const Card& card1, const Card& card2);
	friend bool operator==(const Card& card1, const Card& card2);
public:
    App();
	~App();

	bool init(const std::string title, int xpos, int ypos, int width, int height, int flags);
	bool ttf_init();
	void render();
	void loadTextureOnDeck();
	void drawTexture(SDL_Texture* tex, int x, int y, int width, int heigth, SDL_RendererFlip flip = SDL_FLIP_NONE);
	SDL_Texture* loadTexture(const std::string filePath, SDL_Renderer* renderer);
	SDL_Surface* getSurface(const std::string filePath);
	bool isClickableRectClicked(SDL_Rect* r, int xDown, int yDown, int xUp, int yUp);
	void player1Render();
	void player2Render();
	void player3Render();
	//Thrown cards
	void thrownCardsRender();
	//Current player cards holding message
	void cardsCountMessage();
	//War message
	void warMessage();
	void handleEvents();
	void DestroySDL();
	bool isRunning();

	void playNormalRound();
	bool registerWinner(std::vector<Card>& deskDeck, unsigned winner);
	unsigned getWinner();
	bool hasWar();
	int calcTiePlayers(const Card& c);
	Card getBiggestPlayerCard();
	unsigned findPlayerWithCard(const Card& c);
	void printDeck(const std::vector<Card>& deskDeck) const;
	void playWarRound();
	void playRound();
	bool isGameOver();
	void printPlayer(unsigned i);
	void playWarRoundTest();

	void statsMessage(std::vector<Player>&players);
	bool showStats;


private:
	Deck* deck;
	Player* player1;
	Player* player2;
	Player* player3;
	std::vector<Player> players;
	bool isRoundPlayed;
	unsigned short round;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	void initDeck();
	void setDealButton(bool pressed);

	void setButtonPressedDeal1(bool pr);
	void setButtonPressedDeal2(bool pr);
	void setButtonPressedDeal3(bool pr);
	
	bool getButtonPressedDeal1() const;
	bool getButtonPressedDeal2() const;
	bool getButtonPressedDeal3() const;

	bool getDealButton() const;
	bool buttonPressed;
	bool buttonPressedDeal1;
	bool buttonPressedDeal2;
	bool buttonPressedDeal3;
	GameState state;
	bool running;
	Card c1;
	Card c2;
	Card c3;
	//textures
		//player 1 cards Textures
	SDL_Texture* card1Texture;
		//background texture
	SDL_Texture* backgroundTexture;
		//start texture player1
	SDL_Texture* textStartTexture;
		//deal texture player1
	SDL_Texture* textDealTexture;
		//start texture player2
	SDL_Texture* textStartTexturePlayer2;
		//deal texture player2
	SDL_Texture* textDealTexturePlayer2;
		//start texture player3
	SDL_Texture* textStartTexturePlayer3;
		//deal texture player3
	SDL_Texture* textDealTexturePlayer3;
		//error texture
	SDL_Texture* textErrorTexture;
		//Player 1 text texture
	SDL_Texture* textPlayer1Texture;
		//Player 2 text texture
	SDL_Texture* textPlayer2Texture;
		//Player 3 text texture
	SDL_Texture* textPlayer3Texture;
	//Rects
		//buttons
			//start
	SDL_Rect dRectButtonStart;
	SDL_Rect dRectButtonStartPlayer2;
	SDL_Rect dRectButtonStartPlayer3;
			//deal
	SDL_Rect dRectButtonDeal;
	SDL_Rect dRectButtonDealPlayer2;
	SDL_Rect dRectButtonDealPlayer3;
		//labels
	SDL_Rect dRectTextStart;
	SDL_Rect dRectTextStartPlayer2;
	SDL_Rect dRectTextStartPlayer3;

	SDL_Rect dRectTextDeal;
	SDL_Rect dRectTextDealPlayer2;
	SDL_Rect dRectTextDealPlayer3;

	SDL_Rect dRectTextPlayer1;
	SDL_Rect dRectTextPlayer2;
	SDL_Rect dRectTextPlayer3;

	// stats texture
	SDL_Texture *statsTexture;
	// stats
	SDL_Rect statsButton;

	SDL_Rect dRectTextError;
		//background
	SDL_Rect dRectBackground;
	
	//TTF Font
	TTF_Font* font;

	//mouse coordinates
	int mouseDownX, mouseDownY;
	//texture width and heigth
	int tw, th;
};