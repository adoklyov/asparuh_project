#include "App.h"
#include "Manager.h"
#include <iostream>
#include <string>

const unsigned CARD_DEALED = 10;
const unsigned POSITION_DIFF = 5;

App::App(std::vector<Player> &pl, Manager &mgr)
	: players(pl), manager(mgr), backCardTexture(nullptr), card1Texture(nullptr), card2Texture(nullptr), card3Texture(nullptr), card4Texture(nullptr),
	  card5Texture(nullptr), card6Texture(nullptr), card7Texture(nullptr), card8Texture(nullptr), card9Texture(nullptr),
	  card10Texture(nullptr)

{
	players = pl;
	window = NULL;
	renderer = NULL;
	running = true;
	buttonPressed = false;
	state = GameState::START;
}

App::~App()
{
	DestroySDL();
}
bool App::init(const std::string title, int xpos, int ypos, int width, int height, int flags)
{
	TTF_Init();

	// Load ttf font
	font = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 24);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL initialization failed: " << SDL_GetError() << "\n";
		return false;
	}

	window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags);
	if (nullptr == window)
	{
		std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (nullptr == renderer)
	{
		std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0xCA, 0xAC, 0xFF);
	SDL_Surface *tempSurface = getSurface("/home/default/asparuh_project/assets/cards/background.jpg");
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	dRectBackground = {0, 0, w, h};

	SDL_Texture *backCardTexture = loadTexture("/home/default/asparuh_project/assets/cards/backCard_120.png", renderer);
	if (!backCardTexture)
		return false;

	tempSurface = getSurface("/home/default/asparuh_project/assets/cards/backCard_120.png");

	card1Texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

	SDL_SetTextureAlphaMod(card1Texture, 255);

	SDL_QueryTexture(backCardTexture, 0, 0, &tw, &th);

	SDL_FreeSurface(tempSurface);
	return true;
}

bool App::ttf_init()
{
	if (TTF_Init() == -1)
		return false;

	TTF_Font *font1 = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/Arcade.ttf", 25);
	TTF_Font *font2 = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 45);
	TTF_Font *fontPlayer = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 30);

	if (nullptr == font1 || nullptr == font2 || nullptr == fontPlayer)
		return false;

	SDL_Surface *tempSurfaceText = nullptr;

	// texture start
	tempSurfaceText = TTF_RenderText_Blended(font1, "Start", {0x00, 0x00, 0x00, 0xFF});
	textStartTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture deal
	tempSurfaceText = TTF_RenderText_Blended(font1, "Deal", {0x00, 0x00, 0x00, 0xFF});
	textDealTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture error
	tempSurfaceText = TTF_RenderText_Blended(font2, "Cannot Click", {0x00, 0x00, 0x00, 0xFF});
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

	// getting and setting label textStartTexture for player1
	SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
	dRectTextStart = {5, 355, tw, th};

	// getting and setting label textDealTexture for player1
	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	dRectTextDeal = {95, 355, tw, th};

	// getting and setting label name for player1
	SDL_QueryTexture(textPlayer1Texture, 0, 0, &tw, &th);
	dRectTextPlayer1 = {0, 100, tw, th};

	// getting and setting label textStartTexture for player2
	SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
	dRectTextStartPlayer2 = {340, 455, tw, th};

	// getting and setting label textDealTexture for player2
	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	dRectTextDealPlayer2 = {345, 505, tw, th};

	// getting and setting label name for player2
	SDL_QueryTexture(textPlayer2Texture, 0, 0, &tw, &th);
	dRectTextPlayer2 = {200, 350, tw, th};

	// getting and setting label textStartTexture for player3
	SDL_QueryTexture(textStartTexture, 0, 0, &tw, &th);
	dRectTextStartPlayer3 = {800 - tw - 5 - 90, 355, tw, th};

	// getting and setting label textDealTexture for player3
	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	dRectTextDealPlayer3 = {800 - tw - 5, 355, tw, th};

	// getting and setting label name for player3
	SDL_QueryTexture(textPlayer3Texture, 0, 0, &tw, &th);
	dRectTextPlayer3 = {800 - tw, 100, tw, th};

	// getting and setting Error
	SDL_QueryTexture(textErrorTexture, 0, 0, &tw, &th);
	dRectTextError = {300, 50, tw, th};

	SDL_FreeSurface(tempSurfaceText);
	TTF_CloseFont(font1);

	return true;
}

SDL_Surface *App::getSurface(const std::string filePath)
{
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (nullptr == surface)
	{
		std::cerr << "Failed to load image: " << filePath << ": " << SDL_GetError() << "\n";
		return nullptr;
	}
	return surface;
}

bool App::isClickableRectClicked(SDL_Rect *r, int xDown, int yDown, int xUp, int yUp)
{
	if ((xDown >= r->x && xDown <= (r->x + r->w) && xUp >= r->x && xUp <= (r->x + r->w) &&
		 (yDown >= r->y && yDown <= (r->y + r->h) && yUp >= r->y && yUp <= (r->y + r->h))))
	{
		return true;
	}
	return false;
}

SDL_Texture *App::loadTexture(const std::string filePath, SDL_Renderer *renderer)
{
	SDL_Surface *tempSurface = getSurface(filePath.c_str());

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return texture;
}

void App::render()
{
	SDL_RenderClear(renderer);
	// background
	SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dRectBackground);

	if (GameState::START == state)
	{
		dRectButtonStart = {0, 350, 70, 30};
		SDL_RenderFillRect(renderer, &dRectButtonStart);
		SDL_RenderCopy(renderer, textStartTexture, nullptr, &dRectTextStart);
	}
	else if (GameState::PLAYING == state)
	{

		SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
		player1Render();
		player2Render();
		player3Render();
		cardsCountMessage();

		if (getDealButton())
		{
			thrownCardsRender();
			buttonPressed = true;
			setDealButton(buttonPressed);
		}
	}
	else if (GameState::WAR == state)
	{
		SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
		player1Render();
		player2Render();
		player3Render();
		cardsCountMessage();
		thrownCardsRender();
		warMessage();
	}

	else if (GameState::OVER == state)
	{
	}
	SDL_RenderPresent(renderer);
}

void App::loadTextureOnDeck(Deck &deck)
{
	const char *const arrSuit[4] = {"spades", "hearts", "diamonds", "clubs"};
	const char *const arrFace[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"};
	for (int i = 0; i < deck.getDeck().size(); i++)
	{
		std::string filePathRoot = "/home/default/asparuh_project/assets/cards/";
		filePathRoot.append(arrFace[i % 13]).append("_of_").append(arrSuit[i % 4]).append(".png");
		SDL_Texture *tex = loadTexture(filePathRoot, renderer);
		deck.getDeck()[i].texture = tex;
		if (!deck.getDeck()[i].texture)
			std::cerr << "Failed to load texture for card " << SDL_GetError() << "\n";
	}
}

void App::drawTexture(SDL_Texture *tex, int x, int y, int width, int heigth, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = {0, 0, width, heigth}; // coord from the texture
	SDL_Rect dstRect = {x, y, width, heigth};
	SDL_RenderCopyEx(renderer, tex, &srcRect, &dstRect, 0, 0, flip);
}

void App::handleEvents(Deck &deck, Manager &manager)
{
	SDL_Event event;
	static bool wasPressed = false;
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
		case SDL_MOUSEBUTTONUP:
		{
			int msx, msy;
			if (event.button.button == SDL_BUTTON_LEFT)
			{

				SDL_GetMouseState(&msx, &msy);
				if (isClickableRectClicked(&dRectButtonStart, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonStartPlayer2, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonStartPlayer3, mouseDownX, mouseDownY, msx, msy))
				{
					if (wasPressed)
					{
						std::cerr << "cannot click start again"
								  << "\n";
						SDL_SetTextureAlphaMod(textErrorTexture, 255);
						SDL_RenderCopy(renderer, textErrorTexture, nullptr, &dRectTextError);
						SDL_RenderPresent(renderer);
						SDL_Delay(1000);
						SDL_SetTextureAlphaMod(textErrorTexture, 0);
					}
					else
					{
						wasPressed = true;
						std::cerr << " CLICKED START"
								  << "\n";
						for (unsigned i = 0; i < players.size(); i++)
						{
							players[i].dealCards(deck);
						}
						// deck.print();
						SDL_SetTextureAlphaMod(textStartTexture, 128);
						SDL_SetTextureAlphaMod(card1Texture, 255);
						state = GameState::PLAYING;
						manager.startMatch();
					}
				}
				else if (isClickableRectClicked(&dRectButtonDeal, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonDealPlayer2, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonDealPlayer3, mouseDownX, mouseDownY, msx, msy))
				{

					if (isClickableRectClicked(&dRectButtonDeal, mouseDownX, mouseDownY, msx, msy))
					{
						setButtonPressedDeal1(true);
						setButtonPressedDeal2(false);
						setButtonPressedDeal3(false);

						players[0].setTurn(true);

						c1 = players[0].pullCard();

						buttonPressed = true;
						setDealButton(buttonPressed);
					}
					else if (isClickableRectClicked(&dRectButtonDealPlayer2, mouseDownX, mouseDownY, msx, msy))
					{
						setButtonPressedDeal1(false);
						setButtonPressedDeal2(true);
						setButtonPressedDeal3(false);

						if (!players[1].getPlayerDeck().empty())
						{
							players[1].setTurn(true);
							c2 = players[1].pullCard();
							buttonPressed = true;
						}

						setDealButton(buttonPressed);
					}
					else if (isClickableRectClicked(&dRectButtonDealPlayer3, mouseDownX, mouseDownY, msx, msy))
					{
						setButtonPressedDeal1(false);
						setButtonPressedDeal2(false);
						setButtonPressedDeal3(true);

						players[2].setTurn(true);

						c3 = players[2].pullCard();
						buttonPressed = true;
						setDealButton(buttonPressed);
					}
					if (true == players[0].getTurn() && true == players[1].getTurn() && true == players[2].getTurn())
					{
						players[0].setTurn(false);
						players[1].setTurn(false);
						players[2].setTurn(false);
						std::cerr << "playing a round";
						manager.playRound();
						state = GameState::WAR;
					}
				}
				else
				{
					std::cerr << "NOTHING CLICKED "
							  << "\n";
				}
			}
		};
		break;
		}
	}
}

void App::update()
{
}

void App::DestroySDL()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_Quit();
}

bool App::isRunning()
{
	return running;
}

void App::setDealButton(bool pressed)
{
	buttonPressed = pressed;
}
void App::setButtonPressedDeal1(bool pr)
{
	this->buttonPressedDeal1 = pr;
}
void App::setButtonPressedDeal2(bool pr)
{
	this->buttonPressedDeal2 = pr;
}
void App::setButtonPressedDeal3(bool pr)
{
	this->buttonPressedDeal3 = pr;
}
bool App::getButtonPressedDeal1() const
{
	return this->buttonPressedDeal1;
}
bool App::getButtonPressedDeal2() const
{
	return this->buttonPressedDeal2;
}
bool App::getButtonPressedDeal3() const
{
	return this->buttonPressedDeal3;
}
bool App::getDealButton() const
{
	return buttonPressed;
}

// Player 1 Cards
void App::player1Render()
{
	int numCards = players[0].cntPlayerDeck();

	SDL_Rect dRectPlayer1Cards[10] = {};

	for (int i = 0; i < numCards; i++)
	{
		dRectPlayer1Cards[i] = {0, 150 + i * 2, tw, th};
		SDL_RenderCopy(renderer, card1Texture, nullptr, &dRectPlayer1Cards[i]);
	}

	dRectButtonDeal = {80, 350, 70, 30};
	dRectButtonStart = {0, 350, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonDeal);
	SDL_RenderFillRect(renderer, &dRectButtonStart);
	SDL_RenderCopy(renderer, textStartTexture, nullptr, &dRectTextStart);
	SDL_RenderCopy(renderer, textDealTexture, nullptr, &dRectTextDeal);
	SDL_RenderCopy(renderer, textPlayer1Texture, nullptr, &dRectTextPlayer1);
}

// Player 2 Cards
void App::player2Render()
{
	int numCards = players[1].cntPlayerDeck();

	SDL_Rect dRectPlayer2Cards[10];
	for (int i = 0; i < numCards; i++)
	{
		dRectPlayer2Cards[i] = {200, 400 + i * 2, tw, th};
		SDL_RenderCopy(renderer, card1Texture, nullptr, &dRectPlayer2Cards[i]);
	}

	dRectButtonStartPlayer2 = {330, 450, 70, 30};
	dRectButtonDealPlayer2 = {330, 500, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonStartPlayer2);
	SDL_RenderFillRect(renderer, &dRectButtonDealPlayer2);
	SDL_RenderCopy(renderer, textStartTexture, nullptr, &dRectTextStartPlayer2);
	SDL_RenderCopy(renderer, textDealTexture, nullptr, &dRectTextDealPlayer2);
	SDL_RenderCopy(renderer, textPlayer2Texture, nullptr, &dRectTextPlayer2);
}

// Player 3 Cards
void App::player3Render()
{

	int numCards = players[2].cntPlayerDeck();
	SDL_Rect dRectPlayer3Cards[10] = {};

	for (int i = 0; i < numCards; i++)
	{
		dRectPlayer3Cards[i] = {800 - tw, 150 + i * 2, tw, th};
		SDL_RenderCopy(renderer, card1Texture, nullptr, &dRectPlayer3Cards[i]);
	}

	dRectButtonStartPlayer3 = {650, 350, 70, 30};
	dRectButtonDealPlayer3 = {730, 350, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonStartPlayer3);
	SDL_RenderFillRect(renderer, &dRectButtonDealPlayer3);

	SDL_RenderCopy(renderer, textPlayer3Texture, nullptr, &dRectTextPlayer3);
	SDL_RenderCopy(renderer, textStartTexture, nullptr, &dRectTextStartPlayer3);
	SDL_RenderCopy(renderer, textDealTexture, nullptr, &dRectTextDealPlayer3);
}

void App::thrownCardsRender()
{
	SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
	dRectCard1Thrown = {200, 100, tw, th};
	dRectCard2Thrown = {300, 200, tw, th};
	dRectCard3Thrown = {400, 100, tw, th};
	SDL_RenderCopy(renderer, c1.texture, nullptr, &dRectCard1Thrown);
	SDL_RenderCopy(renderer, c2.texture, nullptr, &dRectCard2Thrown);
	SDL_RenderCopy(renderer, c3.texture, nullptr, &dRectCard3Thrown);
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
	SDL_Rect MessagePos;
	MessagePos.x = 3;
	MessagePos.y = 380;
	MessagePos.w = textW;
	MessagePos.h = textH;

	SDL_RenderCopy(renderer, Message, NULL, &MessagePos);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);

	// Player 2's message
	int player2CardCount = players[1].cntPlayerDeck();
	std::string message2 = "P2 Cards: " + std::to_string(player2CardCount);
	SDL_Surface *surfaceMessage2 = TTF_RenderText_Solid(font, message2.c_str(), textColor);
	SDL_Texture *Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);

	SDL_Rect Message2Pos;
	Message2Pos.x = 340;
	Message2Pos.y = 540;
	Message2Pos.w = textW;
	Message2Pos.h = textH;

	SDL_RenderCopy(renderer, Message2, NULL, &Message2Pos);
	SDL_DestroyTexture(Message2);
	SDL_FreeSurface(surfaceMessage2);

	// Player 3's message
	int player3CardCount = players[2].cntPlayerDeck();
	std::string message3 = "P3 Cards: " + std::to_string(player3CardCount);
	SDL_Surface *surfaceMessage3 = TTF_RenderText_Solid(font, message3.c_str(), textColor);
	SDL_Texture *Message3 = SDL_CreateTextureFromSurface(renderer, surfaceMessage3);

	SDL_Rect Message3Pos;
	Message3Pos.x = 595;
	Message3Pos.y = 400;
	Message3Pos.w = textW;
	Message3Pos.h = textH;

	SDL_RenderCopy(renderer, Message3, NULL, &Message3Pos);
	SDL_DestroyTexture(Message3);
	SDL_FreeSurface(surfaceMessage3);
}

void App::warMessage()
{
	// War message
	std::string warMessage = "WAR!";
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *warSurface = TTF_RenderText_Solid(font, warMessage.c_str(), textColor);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, warSurface);

	SDL_Rect warMessagePos;
	warMessagePos.x = 400;
	warMessagePos.y = 300;
	warMessagePos.h = 40;
	warMessagePos.w = 80;

	SDL_RenderCopy(renderer, Message, NULL, &warMessagePos);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(warSurface);
}