#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Player.h"
#include "Button.h"
#include <pugixml.hpp>
#include <vector>
enum GameState
{
	START,
	PLAYING,
	WAR,
	OVER,
};

class App
{
	friend bool operator>(const Card &card1, const Card &card2);
	friend bool operator<(const Card &card1, const Card &card2);
	friend bool operator==(const Card &card1, const Card &card2);
	friend bool operator!=(const Card &card1, const Card &card2);
	friend bool operator<=(const Card &card1, const Card &card2);
	friend bool operator>=(const Card &card1, const Card &card2);

public:
	App();
	~App();

	bool init(const std::string title, int xpos, int ypos, int width, int height, int flags);
	bool ttf_init();
	void render();
	void loadTextureOnDeck();
	void drawTexture(SDL_Texture *tex, int x, int y, int width, int heigth, SDL_RendererFlip flip = SDL_FLIP_NONE);
	SDL_Texture *loadTexture(const std::string filePath, SDL_Renderer *renderer);
	SDL_Surface *getSurface(const std::string filePath);
	bool isClickableRectClicked(const SDL_Rect& r, int xDown, int yDown, int xUp, int yUp);
	void player1Render();
	void player2Render();
	void player3Render();
	// Thrown cards
	void thrownCardsRender();
	// Current player cards holding message
	void cardsCountMessage();
	// War message
	void warMessage();
	// player winner message
	void winnerOverMessage();
	void winnerMessage();

	void handleEvents();
	void DestroySDL();
	bool isRunning();

	void playNormalRound();
	bool registerWinner(std::vector<Card> &deskDeck, unsigned winner);
	unsigned getWinner();
	bool hasWar();
	int calcTiePlayers(const Card &c);
	Card getBiggestPlayerCardAndSetWinner();
	unsigned findPlayerWithCard(const Card &c);
	void printDeck(const std::vector<Card> &deskDeck) const;
	void playRound();
	void clearTable();

	bool PlayWarRound();
	bool isGameOver();
	void printPlayer(unsigned i);
	//buttons
	void setSettingsForButtonStart(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, SDL_Rect& rect, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer);
	void setSettingsForButtonDeal(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer);
	void setSettingsForButtonDealAll(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer);
	void setSettingsForButtonRestart(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer);
	void setSettingsForButtonStats(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer);

	// stats message
	void statsMessage();

	// XML
	void updateStatsXML();
	void saveStatsXML();

	// game restart
	void restartGame();
	void setWar(bool war);
	bool getWar() const;

	// winner
	void setWinner(int winner);
	auto getWinner() const -> int;

	void setShowStats(bool stats);
	bool getShowStats() const;

	bool handleWarWinConditionsEmpty(const unsigned playerInWar1Index, const unsigned playerInWar2Index, std::vector<Card> &deskDeck);
	void handleMiniDeckConditions(const unsigned playerInWar1Index, const unsigned playerInWar2Index, std::vector<Card> &deskDeck);

private:
	Deck *deck;
	Player *player1;
	Player *player2;
	Player *player3;
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;

	void initOnlyNeededDeck();
	void DestroyTextures();
	void DestroyTexturesForRestart();
	void DestroyTextureOfDeck();

	void setButtonPressedDeal1(bool pr);
	void setButtonPressedDeal2(bool pr);
	void setButtonPressedDeal3(bool pr);

	bool getButtonPressedDeal1() const;
	bool getButtonPressedDeal2() const;
	bool getButtonPressedDeal3() const;

	GameState state;
	Card c1, c2, c3;

	std::vector<Card> dump;
	std::vector<Player> players;
	std::vector<bool> buttonPressedDeal;
	bool buttonPressedDeal1;
	bool buttonPressedDeal2;
	bool buttonPressedDeal3;

	bool wasStartPressed;
	bool showStats;
	bool war = false;
	bool isRoundPlayed;
	bool running;
	unsigned short round;

	// mouse coordinates
	int mouseDownX = 0, mouseDownY = 0;
	// texture width and heigth
	int tw = 0, th = 0;
	// winner
	int winner = -1;

	// textures
	SDL_Texture *tex = nullptr;
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = nullptr;
	// player 1 cards Textures
	SDL_Texture *card1Texture = nullptr;
	// war card textures
	SDL_Texture *warCardTexture = nullptr;
	// background texture
	SDL_Texture *backgroundTexture = nullptr;
	// start from Over state texture
	SDL_Texture *textStartOverTexture = nullptr;
	// start texture player1
	SDL_Texture *textStartTexture = nullptr;
	// deal texture player1
	SDL_Texture *textDealTexture = nullptr;
	// start texture player2
	SDL_Texture *textStartTexturePlayer2 = nullptr;
	// deal texture player2
	SDL_Texture *textDealTexturePlayer2 = nullptr;
	// start texture player3
	SDL_Texture *textStartTexturePlayer3 = nullptr;
	// deal texture player3
	SDL_Texture *textDealTexturePlayer3 = nullptr;
	// error texture
	SDL_Texture *textErrorTexture = nullptr;
	// Player 1 text texture
	SDL_Texture *textPlayer1Texture = nullptr;
	// Player 2 text texture
	SDL_Texture *textPlayer2Texture = nullptr;
	// Player 3 text texture
	SDL_Texture *textPlayer3Texture = nullptr;
	// Rects
	// buttons
	// start
	SDL_Rect dRectButtonStart;
	SDL_Rect dRectButtonStartPlayer2;
	SDL_Rect dRectButtonStartPlayer3;
	// deal
	SDL_Rect dRectButtonDeal;
	SDL_Rect dRectButtonDealPlayer2;
	SDL_Rect dRectButtonDealPlayer3;

	// stats texture
	SDL_Texture *statsTexture = nullptr;
	// stats
	SDL_Rect statsButton;
	// all deal texture
	SDL_Texture *allDealTexture = nullptr;
	// all deal
	SDL_Rect allDeal;
	// reset texture
	SDL_Texture *resetTexture = nullptr;
	// reset
	SDL_Rect reset;
	// TTF Font
	TTF_Font *font = nullptr;

	// //Buttons
	Button start;
	Button deal;
	Button dealAll;
	Button restart;
	Button stats;
};