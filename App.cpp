#include "App.h"
#include <iostream>
#include <cassert>

const unsigned PLAYERS_RENDERING_CARDS_CNT = 300;

App::App()
	: card1Texture(nullptr)
{

	deck = new Deck();
	player1 = new Player();
	player2 = new Player();
	player3 = new Player();

	players.emplace_back(*player1);
	players.emplace_back(*player2);
	players.emplace_back(*player3);

	window = NULL;
	renderer = NULL;
	running = true;

	wasStartPressed = false;
	buttonPressedDeal1 = false;
	buttonPressedDeal2 = true;
	buttonPressedDeal3 = true;
	buttonPressedDeal.emplace_back(buttonPressedDeal1);
	buttonPressedDeal.emplace_back(buttonPressedDeal2);
	buttonPressedDeal.emplace_back(buttonPressedDeal3);

	state = GameState::START;
	setShowStats(false);
	isRoundPlayed = false;
}
App::~App()
{
	DestroySDL();
}

void App::DestroyTextures()
{
	SDL_DestroyTexture(card1Texture);
	SDL_DestroyTexture(warCardTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(textStartOverTexture);
	SDL_DestroyTexture(textStartTexture);
	SDL_DestroyTexture(textDealTexture);
	SDL_DestroyTexture(textStartTexturePlayer2);
	SDL_DestroyTexture(textDealTexturePlayer2);
	SDL_DestroyTexture(textStartTexturePlayer3);
	SDL_DestroyTexture(textDealTexturePlayer3);
	SDL_DestroyTexture(textErrorTexture);
	SDL_DestroyTexture(textPlayer1Texture);
	SDL_DestroyTexture(textPlayer2Texture);
	SDL_DestroyTexture(textPlayer3Texture);
	SDL_DestroyTexture(statsTexture);
	SDL_DestroyTexture(allDealTexture);
	SDL_DestroyTexture(resetTexture);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(tex);
}
void App::DestroyTexturesForRestart()
{
	SDL_DestroyTexture(warCardTexture);
	SDL_DestroyTexture(textStartTexturePlayer2);
	SDL_DestroyTexture(textDealTexturePlayer2);
	SDL_DestroyTexture(textStartTexturePlayer3);
	SDL_DestroyTexture(textDealTexturePlayer3);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(tex);
}
void App::DestroySDL()
{
	DestroyTextureOfDeck();
	delete deck;
	delete player1;
	delete player2;
	delete player3;

	DestroyTextures();
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	SDL_Quit();
}
void App::DestroyTextureOfDeck()
{
	for (unsigned i = 0; i < deck->getDeck().size(); i++)
	{
		SDL_DestroyTexture(deck->getDeck()[i].texture);
	}

	for (unsigned i = 0; i < players.size(); i++)
	{
		for (unsigned j = 0; j < players[i].getPlayerDeck().size(); j++)
		{
			SDL_DestroyTexture(players[i].getPlayerDeck()[j].texture);
		}
	}
}

bool operator>=(const Card &card1, const Card &card2)
{
	return !(card1.value < card2.value);
}
bool operator<=(const Card &card1, const Card &card2)
{
	return !(card1.value > card2.value);
}
bool operator>(const Card &card1, const Card &card2)
{
	return card1.value > card2.value;
}
bool operator<(const Card &card1, const Card &card2)
{
	return card1.value < card2.value;
}
bool operator==(const Card &card1, const Card &card2)
{
	return (card1.value == card2.value);
}
bool operator!=(const Card &card1, const Card &card2)
{
	return (card1.value != card2.value);
}

bool App::init(const std::string title, int xpos, int ypos, int width, int height, int flags)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	updateStatsXML();

	window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags);

	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 0x00, 0xCA, 0xAC, 0xFF);

	SDL_Surface *tempSurface = getSurface("/home/default/asparuh_project/assets/cards/background2.jpg");
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);

	tempSurface = getSurface("/home/default/asparuh_project/assets/cards/backCard_120.png");

	font = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/Arcade.ttf", 28);

	card1Texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

	// Text Color
	SDL_Color textColor = {0, 0, 0};

	// Button textures
	SDL_Texture *active = IMG_LoadTexture(renderer, "/home/default/asparuh_project/assets/button.png");
	SDL_Texture *pressed = IMG_LoadTexture(renderer, "/home/default/asparuh_project/assets/button.png");
	SDL_Texture *inactive = IMG_LoadTexture(renderer, "/home/default/asparuh_project/assets/button.png");

	SDL_Rect rect = {45, 10, 130, 50};

	// Buttons
	setSettingsForButtonStart(active, inactive, pressed, rect, font, textColor, renderer);
	setSettingsForButtonDeal(active,  inactive, pressed,  font, textColor,  renderer);
	setSettingsForButtonDealAll(active,  inactive, pressed,  font, textColor,  renderer);
	setSettingsForButtonRestart(active,  inactive, pressed,  font, textColor,  renderer);
	setSettingsForButtonStats(active,  inactive, pressed,  font, textColor,  renderer);

	SDL_SetTextureAlphaMod(card1Texture, 255);

	initOnlyNeededDeck();

	SDL_FreeSurface(tempSurface);
	return true;
}

void App::setSettingsForButtonStart(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, SDL_Rect& rect, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
{
	start.setActiveTexture(active);
	start.setInactiveTexture(inactive);
	start.setPressedTexture(pressed);
	start.setPosition(rect);
	start.setText("Start", font, textColor, renderer);
}
void App::setSettingsForButtonDeal(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
{
	SDL_Rect rect = {190, 10, 130, 50};
	deal.setActiveTexture(active);
	deal.setInactiveTexture(inactive);
	deal.setPressedTexture(pressed);
	deal.setPosition(rect);
	deal.setText("Deal", font, textColor, renderer);
}
void App::setSettingsForButtonDealAll(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
{
	SDL_Rect rect = {335, 10, 130, 50};
	dealAll.setActiveTexture(active);
	dealAll.setInactiveTexture(inactive);
	dealAll.setPressedTexture(pressed);
	dealAll.setPosition(rect);
	dealAll.setText("Deal All", font, textColor, renderer);
}
void App::setSettingsForButtonRestart(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
{
	SDL_Rect rect = {480, 10, 130, 50};
		restart.setActiveTexture(active);
	restart.setInactiveTexture(inactive);
	restart.setPressedTexture(pressed);
	restart.setPosition(rect);
	restart.setText("Restart", font, textColor, renderer);
}
void App::setSettingsForButtonStats(SDL_Texture* active, SDL_Texture* inactive, SDL_Texture* pressed, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
{
	SDL_Rect rect = {625, 10, 130, 50};
		stats.setActiveTexture(active);
	stats.setInactiveTexture(inactive);
	stats.setPressedTexture(pressed);
	stats.setPosition(rect);
	stats.setText("Stats", font, textColor, renderer);
}
void App::initOnlyNeededDeck()
{
	loadTextureOnDeck();
	deck->shuffle();
	deck->riffleShuffle();

	for (unsigned i = 0; i < players.size(); i++)
	{
		// players[i].setPlayerDeck();
		players[i].dealCards(*deck);
	}
}

bool App::ttf_init()
{
	if (TTF_Init() == -1)
		return false;

	TTF_Font *font1 = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/Arcade.ttf", 25);
	TTF_Font *font2 = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 45);
	TTF_Font *fontStartOver = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 120);
	TTF_Font *fontPlayer = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 30);
	font = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 30);

	if (nullptr == font1 || nullptr == font2 || nullptr == fontPlayer || nullptr == fontStartOver || nullptr == font)
		return false;

	SDL_Surface *tempSurfaceText = nullptr;
	SDL_Color blackColor = {0x00, 0x00, 0x00, 0xFF};

	// texture start from Over state
	tempSurfaceText = TTF_RenderText_Blended(fontStartOver, "Start", {0xAB, 0xCF, 0xFD, 0xFF});
	textStartOverTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);
	// texture start
	tempSurfaceText = TTF_RenderText_Blended(font1, "Start", blackColor);
	textStartTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture deal
	tempSurfaceText = TTF_RenderText_Blended(font1, "Deal", blackColor);
	textDealTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture stats
	tempSurfaceText = TTF_RenderText_Blended(font1, "Stats", blackColor);
	statsTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture all deal
	tempSurfaceText = TTF_RenderText_Blended(font1, "All Deal", blackColor);
	allDealTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture reset
	tempSurfaceText = TTF_RenderText_Blended(font1, "Reset", blackColor);
	resetTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture error
	tempSurfaceText = TTF_RenderText_Blended(font2, "Cannot Click", blackColor);
	textErrorTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// label player1
	tempSurfaceText = TTF_RenderText_Blended(fontPlayer, "Player 1", {0xFF, 0xCC, 0xAA, 0xFF});
	textPlayer1Texture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// label player2
	tempSurfaceText = TTF_RenderText_Blended(fontPlayer, "Player 2", {0xFF, 0xCC, 0xAA, 0xFF});
	textPlayer2Texture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// label player3
	tempSurfaceText = TTF_RenderText_Blended(fontPlayer, "Player 3", {0xFF, 0xCC, 0xAA, 0xFF});
	textPlayer3Texture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	TTF_CloseFont(font1);
	TTF_CloseFont(font2);
	TTF_CloseFont(fontStartOver);
	TTF_CloseFont(fontPlayer);
	SDL_FreeSurface(tempSurfaceText);

	return true;
}
SDL_Surface *App::getSurface(const std::string filePath)
{
	surface = IMG_Load(filePath.c_str());
	if (nullptr == surface)
	{
		std::cerr << "Failed to load image: " << filePath << ": " << SDL_GetError() << "\n";
		return nullptr;
	}
	return surface;
}
void App::loadTextureOnDeck()
{
	const char *const arrSuit[4] = {"clubs", "diamonds", "hearts", "spades"};
	const char *const arrFace[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"};
	for (int i = 0; i < deck->getDeck().size(); i++)
	{
		std::string filePathRoot = "/home/default/asparuh_project/assets/cards/";
		filePathRoot.append(arrFace[i % 13]).append("_of_").append(arrSuit[i % 4]).append(".png");
		tex = loadTexture(filePathRoot, renderer);
		deck->getDeck()[i].texture = tex;
		if (!deck->getDeck()[i].texture)
			std::cerr << "Failed to load texture for card " << SDL_GetError() << "\n";
	}
}
void App::drawTexture(SDL_Texture *tex, int x, int y, int width, int heigth, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = {0, 0, width, heigth}; // coord from the texture
	SDL_Rect dstRect = {x, y, width, heigth};
	SDL_RenderCopyEx(renderer, tex, &srcRect, &dstRect, 0, 0, flip);
}
SDL_Texture *App::loadTexture(const std::string filePath, SDL_Renderer *renderer)
{
	SDL_Surface *tempSurface = getSurface(filePath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return texture;
}
void App::clearTable()
{
	c1.texture = nullptr;
	c2.texture = nullptr;
	c3.texture = nullptr;
	for (unsigned i = 0; i < players.size(); i++)
	{
		for (unsigned j = 0; j < players[i].getMiniPlayerDeck().size(); j++)
		{
			players[i].getMiniPlayerDeck()[j].texture = nullptr;
		}
	}
}
void App::player1Render()
{
	SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);

	int numCards = players[0].cntPlayerDeck();

	for (int i = 0; i < numCards; i++)
	{
		drawTexture(card1Texture, 0, 150 + i * 2, tw, th, SDL_FLIP_NONE);
	}

	// if players 0 and 1 or 0 and 2 are active its always activeplayer[0]
	if (state == GameState::WAR && players[0].isActive() && !players[0].getMiniPlayerDeck().empty())
	{
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		for (int i = 0; i < players[0].getMiniPlayerDeck().size(); i++)
		{
			drawTexture(players[0].getMiniPlayerDeck()[i].texture, 200, 120 + i * 20, tw, th, SDL_FLIP_NONE);
		}
	}

	dRectButtonDeal = {80, 350, 70, 30};
	dRectButtonStart = {0, 350, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonDeal);
	SDL_RenderFillRect(renderer, &dRectButtonStart);

	SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
	drawTexture(textStartTexture, 5, 355, tw, th, SDL_FLIP_NONE);

	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	drawTexture(textDealTexture, 95, 355, tw, th, SDL_FLIP_NONE);

	SDL_QueryTexture(textPlayer1Texture, 0, 0, &tw, &th);
	drawTexture(textPlayer1Texture, 0, 100, tw, th, SDL_FLIP_NONE);
}
void App::player2Render()
{
	SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
	int numCards = players[1].cntPlayerDeck();
	for (int i = 0; i < numCards; i++)
	{
		drawTexture(card1Texture, 220, 400 + i * 2, tw, th, SDL_FLIP_NONE);
	}
	if (state == GameState::WAR && players[1].isActive() && !players[1].getMiniPlayerDeck().empty())
	{
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		for (int i = 0; i < players[1].getMiniPlayerDeck().size(); i++)
		{
			drawTexture(players[1].getMiniPlayerDeck()[i].texture, 300, 220 + i * 20, tw, th, SDL_FLIP_NONE);
		}
	}

	dRectButtonStartPlayer2 = {350, 450, 70, 30};
	dRectButtonDealPlayer2 = {350, 500, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonStartPlayer2);
	SDL_RenderFillRect(renderer, &dRectButtonDealPlayer2);

	SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
	drawTexture(textStartTexture, 360, 455, tw, th, SDL_FLIP_NONE);

	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	drawTexture(textDealTexture, 365, 505, tw, th, SDL_FLIP_NONE);

	SDL_QueryTexture(textPlayer2Texture, 0, 0, &tw, &th);
	drawTexture(textPlayer2Texture, 220, 350, tw, th, SDL_FLIP_NONE);
}
void App::player3Render()
{
	// player 3 render in war of 3
	SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
	int numCards = players[2].cntPlayerDeck();
	for (int i = 0; i < numCards; i++)
	{
		drawTexture(card1Texture, 800 - tw, 150 + i * 2, tw, th, SDL_FLIP_NONE);
	}
	if (state == GameState::WAR && players[2].isActive() && !players[2].getMiniPlayerDeck().empty())
	{
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		for (int i = 0; i < players[2].getMiniPlayerDeck().size(); i++)
		{
			drawTexture(players[2].getMiniPlayerDeck()[i].texture, 400, 120 + i * 20, tw, th, SDL_FLIP_NONE);
		}
	}

	dRectButtonStartPlayer3 = {650, 350, 70, 30};
	dRectButtonDealPlayer3 = {730, 350, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonStartPlayer3);
	SDL_RenderFillRect(renderer, &dRectButtonDealPlayer3);

	SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
	drawTexture(textStartTexture, 800 - tw - 5 - 90, 355, tw, th, SDL_FLIP_NONE);

	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	drawTexture(textDealTexture, 800 - tw - 5, 355, tw, th, SDL_FLIP_NONE);

	SDL_QueryTexture(textPlayer3Texture, 0, 0, &tw, &th);
	drawTexture(textPlayer3Texture, 800 - tw, 100, tw, th, SDL_FLIP_NONE);
}
void App::winnerMessage()
{
	std::string warMessage = "Winner is: " + std::to_string(winner + 1);
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *warSurface = TTF_RenderText_Solid(font, warMessage.c_str(), textColor);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, warSurface);

	SDL_QueryTexture(Message, 0, 0, &tw, &th);
	drawTexture(Message, 500, 60, tw, th, SDL_FLIP_NONE);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(warSurface);
}
void App::thrownCardsRender()
{
	SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
	if (players[0].cntPlayerDeck() == 0 && players[1].cntPlayerDeck() == 0)
	{
		drawTexture(c3.texture, 400, 100, tw, th, SDL_FLIP_NONE);
		return;
	}
	else if (players[0].cntPlayerDeck() == 0 && players[2].cntPlayerDeck() == 0)
	{
		drawTexture(c2.texture, 300, 200, tw, th, SDL_FLIP_NONE);
		return;
	}
	else if (players[1].cntPlayerDeck() == 0 && players[2].cntPlayerDeck() == 0)
	{
		drawTexture(c1.texture, 400, 100, tw, th, SDL_FLIP_NONE);
		return;
	}
	else if (players[0].cntPlayerDeck() == 0)
	{
		drawTexture(c2.texture, 300, 200, tw, th, SDL_FLIP_NONE);
		drawTexture(c3.texture, 400, 100, tw, th, SDL_FLIP_NONE);
		return;
	}
	else if (players[1].cntPlayerDeck() == 0)
	{
		drawTexture(c1.texture, 200, 100, tw, th, SDL_FLIP_NONE);
		drawTexture(c3.texture, 400, 100, tw, th, SDL_FLIP_NONE);
		return;
	}
	else if (players[2].cntPlayerDeck() == 0)
	{
		drawTexture(c1.texture, 200, 100, tw, th, SDL_FLIP_NONE);
		drawTexture(c2.texture, 300, 200, tw, th, SDL_FLIP_NONE);
		return;
	}
	drawTexture(c1.texture, 200, 100, tw, th, SDL_FLIP_NONE);
	drawTexture(c2.texture, 300, 200, tw, th, SDL_FLIP_NONE);
	drawTexture(c3.texture, 400, 100, tw, th, SDL_FLIP_NONE);
}
void App::cardsCountMessage()
{
	// Player 1's message
	int player1CardCount = players[0].cntPlayerDeck();
	std::string message = "P1 Cards: " + std::to_string(player1CardCount);
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), textColor);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	int textW = surfaceMessage->w;
	int textH = surfaceMessage->h;

	drawTexture(Message, 3, 380, textW, textH, SDL_FLIP_NONE);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);

	// Player 2's message
	int player2CardCount = players[1].cntPlayerDeck();
	std::string message2 = "P2 Cards: " + std::to_string(player2CardCount);
	SDL_Surface *surfaceMessage2 = TTF_RenderText_Solid(font, message2.c_str(), textColor);
	SDL_Texture *Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);

	textW = surfaceMessage2->w;
	textH = surfaceMessage2->h;

	drawTexture(Message2, 360, 540, textW, textH, SDL_FLIP_NONE);
	SDL_DestroyTexture(Message2);
	SDL_FreeSurface(surfaceMessage2);

	// Player 3's message
	int player3CardCount = players[2].cntPlayerDeck();
	std::string message3 = "P3 Cards: " + std::to_string(player3CardCount);
	SDL_Surface *surfaceMessage3 = TTF_RenderText_Solid(font, message3.c_str(), textColor);
	SDL_Texture *Message3 = SDL_CreateTextureFromSurface(renderer, surfaceMessage3);

	textW = surfaceMessage3->w;
	textH = surfaceMessage3->h;

	drawTexture(Message3, 595, 400, textW, textH, SDL_FLIP_NONE);
	SDL_DestroyTexture(Message3);
	SDL_FreeSurface(surfaceMessage3);
}
void App::winnerOverMessage()
{
	std::string winnerMessage = "Player " + std::to_string(getWinner() + 1) + " wins! "
																			  "Press Start for a new game!";
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *winnerSurface = TTF_RenderText_Solid(font, winnerMessage.c_str(), textColor);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, winnerSurface);

	SDL_QueryTexture(Message, 0, 0, &tw, &th);
	drawTexture(Message, 50, 75, tw, th, SDL_FLIP_NONE);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(winnerSurface);
}
void App::warMessage()
{
	// War message
	std::string warMessage = "WAR!";
	SDL_Color textColor = {173, 216, 230, 255};
	SDL_Surface *warSurface = TTF_RenderText_Solid(font, warMessage.c_str(), textColor);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, warSurface);

	SDL_QueryTexture(Message, 0, 0, &tw, &th);
	drawTexture(Message, 300, 150, tw, th, SDL_FLIP_NONE);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(warSurface);
}
void App::statsMessage()
{
	// Stats message;
	std::string statsMessage;
	for (size_t i = 0; i < players.size(); ++i)
	{
		statsMessage += "Pl:" + std::to_string(i + 1) +
						" Points: " + std::to_string(players[i].getPoints()) +
						" W: " + std::to_string(players[i].getWins()) +
						" L: " + std::to_string(players[i].getLosses());
	}
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *statsSurface = TTF_RenderText_Solid(font, statsMessage.c_str(), textColor);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, statsSurface);

	SDL_Rect statsPos;
	statsPos.x = 10;
	statsPos.y = 15;
	statsPos.w = 790;
	statsPos.h = 50;

	// drawTexture()...
	SDL_RenderCopy(renderer, Message, NULL, &statsPos);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(statsSurface);
}

void App::render()
{
	statsMessage();
	SDL_RenderClear(renderer);
	// background
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	drawTexture(backgroundTexture, 0, 0, w, h, SDL_FLIP_NONE);
	// statsMessage();
	statsButton = {730, 570, 70, 30};
	SDL_RenderFillRect(renderer, &statsButton);
	drawTexture(statsTexture, 730, 570, 70, 30, SDL_FLIP_NONE);
	// all deal
	allDeal = {730, 539, 70, 30};
	SDL_RenderFillRect(renderer, &allDeal);
	drawTexture(allDealTexture, 730, 539, 70, 30, SDL_FLIP_NONE);
	// reset
	reset = {730, 508, 70, 30};
	SDL_RenderFillRect(renderer, &reset);
	drawTexture(resetTexture, 730, 508, 70, 30, SDL_FLIP_NONE);

	// New Buttons
	start.render(renderer);
	deal.render(renderer);
	dealAll.render(renderer);
	restart.render(renderer);
	stats.render(renderer);

	if (GameState::START == state)
	{
		dRectButtonStart = {0, 350, 70, 30};
		SDL_RenderFillRect(renderer, &dRectButtonStart);
		SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
		drawTexture(textStartTexture, 5, 355, tw, th, SDL_FLIP_NONE);
	}

	else if (GameState::PLAYING == state)
	{
		thrownCardsRender();
		player1Render();
		player2Render();
		player3Render();
		cardsCountMessage();
		winnerMessage();
	}
	else if (GameState::WAR == state)
	{
		SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
		thrownCardsRender();
		player1Render();
		player2Render();
		player3Render();
		cardsCountMessage();
		winnerMessage();

		warMessage();
	}
	else if (GameState::OVER == state)
	{
		SDL_QueryTexture(textStartOverTexture, 0, 0, &tw, &th);
		drawTexture(textStartOverTexture, 330, 270, tw, th, SDL_FLIP_NONE);
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (players[i].getIsDefeated())
			{
				players[i].incrementLosses();
				players[i].setIsDefeated(false);
			}
		}
		saveStatsXML();
		winnerOverMessage();
	}

	if (getShowStats())
	{
		statsMessage();
	}

	SDL_RenderPresent(renderer);
}

bool App::isClickableRectClicked(const SDL_Rect& r, int xDown, int yDown, int xUp, int yUp)
{
	if ((xDown >= r.x && xDown <= (r.x + r.w) && xUp >= r.x && xUp <= (r.x + r.w) &&
		 (yDown >= r.y && yDown <= (r.y + r.h) && yUp >= r.y && yUp <= (r.y + r.h))))
	{
		return true;
	}
	return false;
}
bool App::registerWinner(std::vector<Card> &deskDeck, unsigned winner)
{
	std::cerr << "Winner is: " << winner + 1 << '\n';
	this->winner = winner;
	for (unsigned i = 0; i < deskDeck.size(); i++)
	{
		players[winner].getPlayerDeck().push_back(deskDeck[i]);
	}
	return true;
}
int App::calcTiePlayers(const Card &c)
{
	int count = 0;
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == c)
		{
			count++;
		}
	}
	return count;
}
Card App::getBiggestPlayerCardAndSetWinner()
{
	Card biggest(Suit::Hearts, Face::Two, 2);

	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() >= biggest)
		{
			biggest = players[i].getPlayerDeck().front();
			winner = i;
		}
	}

	return biggest;
}
bool App::hasWar()
{
	Card c = getBiggestPlayerCardAndSetWinner();
	if (calcTiePlayers(c) > 1)
	{
		return true;
	}
	return false;
}
unsigned App::findPlayerWithCard(const Card &c)
{
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == c)
		{
			if (state != GameState::OVER)
				players[i].incrementPoint(c);
			return i;
		}
	}
	std::cerr << "Invalid player\n";
	return -1;
}
unsigned App::getWinner()
{
	if (hasWar())
	{
		std::cerr << "Invalid player\n";
		return -1; // invalid player
	}
	getBiggestPlayerCardAndSetWinner();
	return winner;
}
bool App::handleWarWinConditionsEmpty(const unsigned playerInWar1Index, const unsigned playerInWar2Index, std::vector<Card> &deskDeck)
{
	if (players[playerInWar1Index].getPlayerDeck().empty() && players[playerInWar1Index].getMiniPlayerDeck().empty() || players[playerInWar2Index].getPlayerDeck().empty() && players[playerInWar2Index].getMiniPlayerDeck().empty())
	{
		if (war)
			for (unsigned i = 0; i < dump.size(); i++)
				deskDeck.push_back(dump[i]);
		else
			dump.clear();

		if (players[playerInWar1Index].getPlayerDeck().empty())
		{
			registerWinner(deskDeck, playerInWar2Index);
			return true;
		}
		else if (players[playerInWar2Index].getPlayerDeck().empty())
		{
			registerWinner(deskDeck, playerInWar1Index);
			return true;
		}

		setWar(false);
	}
	return false;
}
void App::handleMiniDeckConditions(const unsigned playerInWar1Index, const unsigned playerInWar2Index, std::vector<Card> &deskDeck)
{
	if (players[playerInWar1Index].getMiniPlayerDeck().size() > players[playerInWar2Index].getMiniPlayerDeck().size())
	{
		if (war)
			for (unsigned i = 0; i < dump.size(); i++)
				deskDeck.push_back(dump[i]);
		else
			dump.clear();

		registerWinner(deskDeck, playerInWar1Index);
		setWar(false);
	}
	else if (players[playerInWar1Index].getMiniPlayerDeck().size() < players[playerInWar2Index].getMiniPlayerDeck().size())
	{
		if (war)
			for (unsigned i = 0; i < dump.size(); i++)
				deskDeck.push_back(dump[i]);
		else
			dump.clear();

		registerWinner(deskDeck, playerInWar2Index);
		setWar(false);
	}
	else if (players[playerInWar1Index].getMiniPlayerDeck().back() != players[playerInWar2Index].getMiniPlayerDeck().back())
	{
		if (war)
			for (unsigned i = 0; i < dump.size(); i++)
				deskDeck.push_back(dump[i]);
		else
			dump.clear();

		Card biggest(Suit::Hearts, Face::Two, 2);
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (players[i].isActive())
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() >= biggest)
				{
					biggest = players[i].getMiniPlayerDeck().back();
					winner = i;
				}
			}
		}

		registerWinner(deskDeck, winner);
		setWar(false);
	}
	else
	{
		std::cout << "\n\tWAR AGAIN\n";
		war = true;
		PlayWarRound();
	}
}
void App::playRound()
{

	if (isGameOver())
	{
		state = GameState::OVER;
		return;
	}
	// round++;

	if (hasWar())
	{
		// Transition is successful, even when I changed it to OVER
		state = GameState::WAR;
		for (unsigned i = 0; i < players.size(); i++)
		{
			players[i].getMiniPlayerDeck().clear();
		}
		// Playing returns false for still in War and true for whether the war is done
		PlayWarRound();
	}
	else
	{
		state = GameState::PLAYING;
		playNormalRound();
	}

	if (isGameOver())
	{
		state = GameState::OVER;
		return;
	}
}
bool App::PlayWarRound()
{
	// koi players imat ednakvi karti, za da se opredeli activityto
	int playersInWarCnt = 0;
	Card strongestCardInWar(Suit::Hearts, Face::Two, 2);
	Card strongestCard(Suit::Hearts, Face::Two, 2);
	std::vector<Card> deskDeck;

	if (war) // if there is second war in the first war
	{
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (players[i].isActive())
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() >= strongestCardInWar)
				{
					strongestCardInWar = players[i].getMiniPlayerDeck().back();
				}
			}
		}

		for (unsigned i = 0; i < players.size(); i++)
		{
			if (players[i].isActive())
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() == strongestCardInWar)
				{
					playersInWarCnt++;
				}
			}
		}
	}
	else // for the first round of war
	{
		strongestCard = getBiggestPlayerCardAndSetWinner();
		playersInWarCnt = calcTiePlayers(strongestCard);
	}

	// clearing and setting the players activity to false
	for (unsigned i = 0; i < players.size(); i++)
	{
		players[i].setActive(false);
	}

	// checking how many players are in war, if one return
	if (playersInWarCnt == 1)
	{
		std::cout << "There is no war what are you doing here\n";
		return true; // the war is done
	}

	// checking if the cards are the strongest in players deck
	if (war)
	{
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() == strongestCardInWar)
			{
				players[i].setActive(true);
			}
		}
	}
	else
	{
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == strongestCard)
			{
				players[i].setActive(true);
			}
		}
	}

	if (!war)
	{
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (!players[i].getPlayerDeck().empty())
			{
				deskDeck.push_back(players[i].pullCard());
			}
		}
		for (unsigned i = 0; i < deskDeck.size(); i++)
		{
			dump.push_back(deskDeck[i]);
		}
	}

	for (unsigned i = 0; i < players.size(); i++)
	{
		if (players[i].isActive())
		{
			for (unsigned j = 0; j < players[i].getMiniPlayerDeck().size(); j++)
			{
				deskDeck.push_back(players[i].getMiniPlayerDeck()[j]);
			}
		}
	}

	std::cerr << "=======================\n";
	std::cerr << "\tDesk Deck\n";
	printDeck(deskDeck);
	std::cerr << "\tPlayers:\n";
	for (unsigned i = 0; i < players.size(); i++)
	{
		printPlayer(i);
	}

	for (unsigned i = 0; i < players.size(); i++)
	{
		if (players[i].isActive())
		{
			for (unsigned j = 0; j < 3; j++) // throwing 3 cards from each
			{
				if (players[i].getPlayerDeck().empty())
					break;
				players[i].getMiniPlayerDeck().push_back(players[i].getPlayerDeck().front());
				deskDeck.push_back(players[i].pullCard());
			}
		}
	}
	bool player1And2And3 = players[0].isActive() && players[1].isActive() && players[2].isActive();
	if (player1And2And3)
	{
		std::cerr << "Here we are in player 1, 2 and 3 isActive\n";
		// printing
		for (unsigned i = 0; i < players.size(); i++)
		{
			std::cout << "Player " << i + 1 << " is playing:\n";
			printDeck(players[i].getMiniPlayerDeck());
		}

		for (int i = 0; i < players.size() - 1; ++i)
			for (int j = i + 1; j < players.size(); ++j)
				handleWarWinConditionsEmpty(i, j, deskDeck);

		if (players[0].getMiniPlayerDeck().back() != players[1].getMiniPlayerDeck().back() &&
			players[0].getMiniPlayerDeck().back() != players[2].getMiniPlayerDeck().back() &&
			players[1].getMiniPlayerDeck().back() != players[2].getMiniPlayerDeck().back())
		{
			if (war)
			{
				for (unsigned i = 0; i < dump.size(); i++)
					deskDeck.push_back(dump[i]);
			}
			Card biggest(Suit::Hearts, Face::Two, 2);
			biggest.value = 2;
			int winner = -1;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (players[i].isActive())
				{
					if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() > biggest)
					{
						biggest = players[i].getMiniPlayerDeck().back();
						winner = i;
					}
				}
			}
			players[winner].incrementPoint(biggest);

			registerWinner(deskDeck, winner);
			setWar(false);

			return true; // the war is done
		}
		else
		{
			if (players[0].isActive() && players[1].isActive())
			{
				std::cerr << "Here we are in player 1 and 2 isActive\n";
				// printing
				for (unsigned i = 0; i < players.size() - 1; i++)
				{
					std::cout << "Player " << i + 1 << " is playing:\n";
					printDeck(players[i].getMiniPlayerDeck());
				}

				if (handleWarWinConditionsEmpty(0, 1, deskDeck))
					return true;
				handleMiniDeckConditions(0, 1, deskDeck);
			}
			else if (players[0].isActive() && players[2].isActive())
			{
				std::cerr << "here are playing player1 and player 3\n";
				for (unsigned i = 0; i < players.size(); i++)
				{
					if (i % 2 == 0)
					{
						std::cout << "Player " << i + 1 << " is playing:\n";
						printDeck(players[i].getMiniPlayerDeck());
					}
				}

				if (handleWarWinConditionsEmpty(0, 2, deskDeck))
					return true;
				handleMiniDeckConditions(0, 2, deskDeck);
			}
			else if (players[1].isActive() && players[2].isActive())
			{
				std::cerr << "Here are playing player 2 and 3\n";
				for (unsigned i = 1; i < players.size(); i++)
				{
					std::cerr << "Player " << i + 1 << " is playing:\n";
					printDeck(players[i].getMiniPlayerDeck());
				}

				if (handleWarWinConditionsEmpty(1, 2, deskDeck))
					return true;
				handleMiniDeckConditions(1, 2, deskDeck);
			}
			else
			{
				std::cout << "\n\tERROR\n";
				setWar(false);
				return false;
			}
		}
	}
	else if (players[0].isActive() && players[1].isActive())
	{
		std::cerr << "Here we are in player 1 and 2 isActive\n";
		// printing
		for (unsigned i = 0; i < players.size() - 1; i++)
		{
			std::cout << "Player " << i + 1 << " is playing:\n";
			printDeck(players[i].getMiniPlayerDeck());
		}

		if (handleWarWinConditionsEmpty(0, 1, deskDeck))
			return true; // the war is done

		handleMiniDeckConditions(0, 1, deskDeck);
	}
	else if (players[0].isActive() && players[2].isActive())
	{
		std::cerr << "here are playing player1 and player 3\n";
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (i % 2 == 0)
			{
				std::cout << "Player " << i + 1 << " is playing:\n";
				printDeck(players[i].getMiniPlayerDeck());
			}
		}

		if (handleWarWinConditionsEmpty(0, 2, deskDeck))
			return true;

		handleMiniDeckConditions(0, 2, deskDeck);
	}
	else if (players[1].isActive() && players[2].isActive())
	{
		std::cerr << "Here are playing player 2 and 3\n";
		for (unsigned i = 1; i < players.size(); i++)
		{
			std::cout << "Player " << i + 1 << " is playing:\n";
			printDeck(players[i].getMiniPlayerDeck());
		}

		if (handleWarWinConditionsEmpty(1, 2, deskDeck))
			return true;
		handleMiniDeckConditions(1, 2, deskDeck);
	}
	else
	{
		std::cout << "\n\tERROR\n";
		setWar(false);

		return false;
	}

	setWar(false);

	return true;
}
void App::playNormalRound()
{
	unsigned winner = getWinner();

	std::vector<Card> deskDeck;
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (players[i].cntPlayerDeck() > 0)
		{
			Card c = players[i].pullCard();
			deskDeck.push_back(c);
		}
	}
	std::cerr << "Desk deck:\n";
	printDeck(deskDeck);

	// play the game

	registerWinner(deskDeck, winner);

	for (int i = 0; i < players.size(); i++)
	{
		std::cerr << "Player " << (i + 1) << ", Cards number: " << players[i].cntPlayerDeck() << '\n';
	}
}

void App::handleEvents()
{

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			int msx, msy;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&msx, &msy);
				mouseDownX = msx;
				mouseDownY = msy;
			}
		};
		break;
		case SDL_MOUSEMOTION:
		{
			int msx, msy;
			SDL_GetMouseState(&msx, &msy);
			mouseDownX = msx;
			mouseDownY = msy;
			if ((mouseDownX >= dRectButtonStart.x && mouseDownX <= (dRectButtonStart.x + dRectButtonStart.w) && 
				msx >= dRectButtonStart.x && msx <= (dRectButtonStart.x + dRectButtonStart.w) &&
				 (mouseDownY >= dRectButtonStart.y && mouseDownY <= (dRectButtonStart.y + dRectButtonStart.h) && 
				msy >= dRectButtonStart.y && msy <= (dRectButtonStart.y + dRectButtonStart.h))))
			{
				std::cout << std::boolalpha << true;
			}
		}
		break;
	case SDL_MOUSEWHEEL:
	{
		winnerMessage();
		players[0].setTurn(true);
		players[1].setTurn(true);
		players[2].setTurn(true);

		c1 = players[0].getPlayerDeck().front();
		c2 = players[1].getPlayerDeck().front();
		c3 = players[2].getPlayerDeck().front();

		isRoundPlayed = true;
		saveStatsXML();
		std::cerr << "---------------------------------\n";
		int sum = 0;
		for (unsigned i = 0; i < players.size(); i++)
		{
			sum += players[i].cntPlayerDeck();
		}
		std::cerr << "\nSUM " << sum;
		std::cerr << "\nnew  round\n";
		playRound();
	};
	break;
	case SDL_MOUSEBUTTONUP:
	{
		int msx, msy;
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			// Play War Round returns true and continue the game as War
			// Waiting to press a button

			SDL_GetMouseState(&msx, &msy);
			SDL_QueryTexture(textStartOverTexture, 0, 0, &tw, &th);
			SDL_Rect dRectStartOver = {330, 270, tw, th};
			if (isClickableRectClicked(dRectStartOver, mouseDownX, mouseDownY, msx, msy) && state == GameState::OVER)
			{
				restartGame();
			}
			if (isClickableRectClicked(start.getButtonRect(), mouseDownX, mouseDownY, msx, msy)/* || isClickableRectClicked(dRectButtonStart, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(dRectButtonStartPlayer2, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(dRectButtonStartPlayer3, mouseDownX, mouseDownY, msx, msy)*/)
			{
				if (!wasStartPressed)
				{
					wasStartPressed = true;
					std::cerr << " CLICKED START"
							  << "\n";
					SDL_SetTextureAlphaMod(textStartTexture, 128);
					SDL_SetTextureAlphaMod(card1Texture, 255);
					state = GameState::PLAYING;
				}
			}
			else if (isClickableRectClicked(dRectButtonDeal, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(dRectButtonDealPlayer2, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(dRectButtonDealPlayer3, mouseDownX, mouseDownY, msx, msy))
			{
				// Having a boolean helps with indicating whether there is a war or not
				// a possible if structure here
				if (players[0].cntPlayerDeck() == 0)
				{
					players[0].setTurn(true);
					setButtonPressedDeal1(true);
					setButtonPressedDeal2(false);
					setButtonPressedDeal3(false);
				}
				else if (players[1].cntPlayerDeck() == 0)
				{
					players[1].setTurn(true);
					setButtonPressedDeal1(false);
					setButtonPressedDeal2(true);
					setButtonPressedDeal3(false);
				}
				else if (players[2].cntPlayerDeck() == 0)
				{
					players[2].setTurn(true);
					setButtonPressedDeal1(false);
					setButtonPressedDeal2(false);
					setButtonPressedDeal3(true);
				}
				if (isClickableRectClicked(dRectButtonDeal, mouseDownX, mouseDownY, msx, msy) && !getButtonPressedDeal1())
				{
					clearTable();
					players[0].setTurn(true);

					c1 = players[0].getPlayerDeck().front();
					setButtonPressedDeal1(true);
					setButtonPressedDeal2(false);
				}
				else if (isClickableRectClicked(dRectButtonDealPlayer2, mouseDownX, mouseDownY, msx, msy) && !getButtonPressedDeal2())
				{

					players[1].setTurn(true);
					c2 = players[1].getPlayerDeck().front();
					setButtonPressedDeal3(false);
				}
				else if (isClickableRectClicked(dRectButtonDealPlayer3, mouseDownX, mouseDownY, msx, msy) && !getButtonPressedDeal3())
				{
					players[2].setTurn(true);

					c3 = players[2].getPlayerDeck().front();
					setButtonPressedDeal1(false);
					setButtonPressedDeal2(true);
					setButtonPressedDeal3(true);
				}
				if (GameState::WAR == state && isClickableRectClicked(dRectButtonDeal, mouseDownX, mouseDownY, msx, msy))
				{
					state = GameState::PLAYING;
					isRoundPlayed = false;
				}
				if (true == players[0].getTurn() && true == players[1].getTurn() && true == players[2].getTurn())
				{

					players[0].setTurn(false);
					players[1].setTurn(false);
					players[2].setTurn(false);
					saveStatsXML();
					std::cerr << "---------------------------------\n";
					int sum = 0;
					for (unsigned i = 0; i < players.size(); i++)
					{
						sum += players[i].cntPlayerDeck();
					}
					std::cerr << "SUM " << sum;
					std::cerr << "\nnew  round\n";
					isRoundPlayed = true;

					playRound();
				}
				// conditions to put actives on false so that we can proceed with rendering based on the war players
			}

			else if (isClickableRectClicked(stats.getButtonRect(), mouseDownX, mouseDownY, msx, msy))
			{
				setShowStats(!getShowStats());
				statsMessage();
			}
			else if (isClickableRectClicked(dealAll.getButtonRect(), mouseDownX, mouseDownY, msx, msy))
			{
				players[0].setTurn(true);
				players[1].setTurn(true);
				players[2].setTurn(true);

				c1 = players[0].getPlayerDeck().front();
				c2 = players[1].getPlayerDeck().front();
				c3 = players[2].getPlayerDeck().front();

				isRoundPlayed = true;
				saveStatsXML();
				std::cerr << "---------------------------------\n";
				int sum = 0;
				for (unsigned i = 0; i < players.size(); i++)
				{
					sum += players[i].cntPlayerDeck();
				}
				std::cerr << "\nSUM " << sum;
				std::cerr << "\nnew  round\n";
				// for (unsigned i = 0; i < !players[0].isActive() || !players[1].isActive() || !players[2].isActive(); i++)
				// {
				// 	playRound();
				// 	if(i%5 == 0)
				// 		restartGame();
				// }
				playRound();
			}
			else if (isClickableRectClicked(restart.getButtonRect(), mouseDownX, mouseDownY, msx, msy))
			{
				restartGame();
			}

			else
			{
				std::cerr << "NOTHING CLICKED "
						  << "\n";
			}
		}

		// if(start)
		// {

		// }
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			restartGame();
		}
	};
	break;
	}
}
}

void App::printDeck(const std::vector<Card> &deskDeck) const
{
	for (unsigned i = 0; i < deskDeck.size(); i++)
	{
		std::cerr << "Face: ";
		switch (deskDeck[i].face)
		{
		case Face::Two:
			std::cerr << "2";
			break;
		case Face::Three:
			std::cerr << "3";
			break;
		case Face::Four:
			std::cerr << "4";
			break;
		case Face::Five:
			std::cerr << "5";
			break;
		case Face::Six:
			std::cerr << "6";
			break;
		case Face::Seven:
			std::cerr << "7";
			break;
		case Face::Eight:
			std::cerr << "8";
			break;
		case Face::Nine:
			std::cerr << "9";
			break;
		case Face::Ten:
			std::cerr << "T";
			break;
		case Face::Jack:
			std::cerr << "J";
			break;
		case Face::Queen:
			std::cerr << "Q";
			break;
		case Face::King:
			std::cerr << "K";
			break;
		case Face::Ace:
			std::cerr << "A";
			break;
		default:
			break;
		}
		std::cerr << " Suit: ";
		switch (deskDeck[i].suit)
		{
		case Suit::Clubs:
			std::cerr << "Clubs";
			break;
		case Suit::Diamonds:
			std::cerr << "Diamonds";
			break;
		case Suit::Hearts:
			std::cerr << "Hearts";
			break;
		case Suit::Spades:
			std::cerr << "Spades";
			break;
		default:
			break;
		}
		std::cerr << '\n';
	}
}
void App::printPlayer(unsigned i)
{
	std::cerr << "Player " << (i + 1) << ", Cards number: " << players[i].cntPlayerDeck() << '\n';
	std::cerr << "Cards:" << '\n';
	players[i].printCards();
}

bool App::isGameOver()
{
	// cerr is being printed, but the state is not being changed
	Player &player1 = players[0];
	Player &player2 = players[1];
	Player &player3 = players[2];

	if (player1.cntPlayerDeck() == 0 && player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Tie!\n";
		return true;
	}
	else if (player1.cntPlayerDeck() == 0 && player2.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 3 wins!\n";
		setWinner(3);

		if (state == GameState::OVER)
			player3.incrementWins();
		return true;
	}
	else if (player1.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 2 wins!\n";
		setWinner(2);
		state = GameState::OVER;
		if (state == GameState::OVER)
			player2.incrementWins();

		return true;
	}
	else if (player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 1 wins!\n";
		setWinner(1);
		state = GameState::OVER;
		if (state == GameState::OVER)
			player1.incrementWins();

		return true;
	}
	if (player1.cntPlayerDeck() == 0 && player2.cntPlayerDeck() != 0 && player3.cntPlayerDeck() != 0)
	{
		player1.setIsDefeated(true);
	}
	else if (player1.cntPlayerDeck() != 0 && player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() != 0)
	{
		player2.setIsDefeated(true);
	}
	else if (player1.cntPlayerDeck() != 0 && player2.cntPlayerDeck() != 0 && player3.cntPlayerDeck() == 0)
	{
		player3.setIsDefeated(true);
	}
	return false;
}

void App::updateStatsXML()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("/home/default/asparuh_project/stats.xml");

	int id = 1;
	for (auto &player : players)
	{
		auto node = doc.child("Stats").find_child_by_attribute("Player", "id", std::to_string(id).c_str());
		if (node)
		{
			player.setPoints(node.child("Points").text().as_int());
			player.setWins(node.child("Wins").text().as_int());
			player.setLosses(node.child("Losses").text().as_int());
		}
		++id;
	}
}
void App::saveStatsXML()
{
	pugi::xml_document doc;
	auto root = doc.append_child("Stats");

	int id = 1;
	for (auto &player : players)
	{
		auto node = doc.child("Stats").find_child_by_attribute("Player", "id", std::to_string(id).c_str());
		node = root.append_child("Player");
		node.append_attribute("id") = std::to_string(id).c_str();

		auto pointsNode = node.child("Points");
		pointsNode = node.append_child("Points");
		pointsNode.text() = player.getPoints();

		auto winsNode = node.child("Wins");
		winsNode = node.append_child("Wins");
		winsNode.text() = player.getWins();

		auto lossesNode = node.child("Losses");
		lossesNode = node.append_child("Losses");
		lossesNode.text() = player.getLosses();

		++id;
	}

	doc.save_file("/home/default/asparuh_project/stats.xml");
}

// Restart game
void App::restartGame()
{
	DestroyTextureOfDeck();
	DestroyTexturesForRestart();

	delete deck;
	delete player1;
	delete player2;
	delete player3;
	// SDL_FreeSurface(surface);

	players.clear();
	buttonPressedDeal.clear();

	deck = new Deck();
	player1 = new Player();
	player2 = new Player();
	player3 = new Player();

	// saveStatsXML();

	players.emplace_back(*player1);
	players.emplace_back(*player2);
	players.emplace_back(*player3);
	initOnlyNeededDeck();

	for (unsigned i = 0; i < players.size(); i++)
	{
		players[i].setActive(false);
	}
	updateStatsXML();

	wasStartPressed = false;
	winner = -1;
	buttonPressedDeal1 = false;
	buttonPressedDeal2 = true;
	buttonPressedDeal3 = true;

	buttonPressedDeal.emplace_back(buttonPressedDeal1);
	buttonPressedDeal.emplace_back(buttonPressedDeal2);
	buttonPressedDeal.emplace_back(buttonPressedDeal3);

	state = GameState::PLAYING;
	clearTable();
	isRoundPlayed = false;
	setWar(false);
	dump.clear();
	SDL_SetTextureAlphaMod(textStartTexture, 255);
}

void App::setButtonPressedDeal1(bool pr)
{
	buttonPressedDeal[0] = pr;
}
bool App::getButtonPressedDeal1() const
{
	return buttonPressedDeal[0];
}
void App::setButtonPressedDeal2(bool pr)
{
	buttonPressedDeal[1] = pr;
}
bool App::getButtonPressedDeal2() const
{
	return buttonPressedDeal[1];
}
void App::setButtonPressedDeal3(bool pr)
{
	buttonPressedDeal[2] = pr;
}
bool App::getButtonPressedDeal3() const
{
	return buttonPressedDeal[2];
}
void App::setShowStats(bool stats)
{
	this->showStats = stats;
}
bool App::getShowStats() const
{
	return this->showStats;
}
void App::setWar(bool war)
{
	this->war = war;
}
bool App::getWar() const
{
	return this->war;
}
void App::setWinner(int winner)
{
	this->winner = winner;
}
int App::getWinner() const
{
	return winner;
}
bool App::isRunning()
{
	return running;
}