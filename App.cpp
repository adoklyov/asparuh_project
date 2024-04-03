#include "App.h"
#include <iostream>

const unsigned PLAYERS_RENDERING_CARDS_CNT = 30;

App::App()
	: card1Texture(nullptr), warCardPlayer1(), warCardPlayer2(), warCardPlayer3()

{

	deck = new Deck();
	player1 = new Player();
	player2 = new Player();
	player3 = new Player();

	players.push_back(*player1);
	players.push_back(*player2);
	players.push_back(*player3);

	window = NULL;
	renderer = NULL;
	running = true;

	wasStartPressed = false;
	buttonPressedDeal1 = false;
	buttonPressedDeal2 = true;
	buttonPressedDeal3 = true;
	buttonPressedDeal.push_back(buttonPressedDeal1);
	buttonPressedDeal.push_back(buttonPressedDeal2);
	buttonPressedDeal.push_back(buttonPressedDeal3);

	state = GameState::START;
	buttonPressed = false;
	showStats = false;
	isRoundPlayed = false;
}

App::~App()
{
	DestroySDL();
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
	updateStatsXML(players);

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

	initDeck();

	SDL_FreeSurface(tempSurface);
	return true;
}

void App::initDeck()
{
	loadTextureOnDeck();
	deck->shuffle();
	deck->riffleShuffle();

	for (unsigned i = 0; i < players.size(); i++)
	{
		players[i].dealCards(*deck);
	}
}

bool App::ttf_init()
{
	if (TTF_Init() == -1)
		return false;

	TTF_Font *font1 = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/Arcade.ttf", 25);
	TTF_Font *font2 = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 45);
	TTF_Font *fontPlayer = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 30);
	font = TTF_OpenFont("/home/default/asparuh_project/assets/fonts/segoepr.ttf", 30);

	if (nullptr == font1 || nullptr == font2 || nullptr == fontPlayer)
		return false;

	SDL_Surface *tempSurfaceText = nullptr;

	// texture start
	tempSurfaceText = TTF_RenderText_Blended(font1, "Start", {0x00, 0x00, 0x00, 0xFF});
	textStartTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture deal
	tempSurfaceText = TTF_RenderText_Blended(font1, "Deal", {0x00, 0x00, 0x00, 0xFF});
	textDealTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture stats
	tempSurfaceText = TTF_RenderText_Blended(font1, "Stats", {0x00, 0x00, 0x00, 0xFF});
	statsTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture all deal
	tempSurfaceText = TTF_RenderText_Blended(font1, "All Deal", {0x00, 0x00, 0x00, 0xFF});
	allDealTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	// texture reset
	tempSurfaceText = TTF_RenderText_Blended(font1, "Reset", {0x00, 0x00, 0x00, 0xFF});
	resetTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

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
	dRectTextStartPlayer2 = {360, 455, tw, th};

	// getting and setting label textDealTexture for player2
	SDL_QueryTexture(textDealTexture, 0, 0, &tw, &th);
	dRectTextDealPlayer2 = {365, 505, tw, th};

	// getting and setting label name for player2
	SDL_QueryTexture(textPlayer2Texture, 0, 0, &tw, &th);
	dRectTextPlayer2 = {220, 350, tw, th};

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
	statsMessage(players);
	SDL_RenderClear(renderer);
	// background
	SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dRectBackground);
	// statsMessage();
	statsButton = {730, 570, 70, 30};
	SDL_RenderFillRect(renderer, &statsButton);
	SDL_RenderCopy(renderer, statsTexture, nullptr, &statsButton);
	// all deal
	allDeal = {730, 540, 70, 30};
	SDL_RenderFillRect(renderer, &allDeal);
	SDL_RenderCopy(renderer, allDealTexture, nullptr, &allDeal);
	// reset
	reset = {730, 510, 70, 30};
	SDL_RenderFillRect(renderer, &reset);
	SDL_RenderCopy(renderer, resetTexture, nullptr, &reset);

	// //Winner Message test rendering
	// winnerMessage();


	if (GameState::START == state)
	{
		dRectButtonStart = {0, 350, 70, 30};
		SDL_RenderFillRect(renderer, &dRectButtonStart);
		SDL_RenderCopy(renderer, textStartTexture, nullptr, &dRectTextStart);
	}
	else if (GameState::PLAYING == state)
	{
		thrownCardsRender();
		// SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
		player1Render();
		player2Render();
		player3Render();
		cardsCountMessage();

		// if (getDealButton())
		// {
		buttonPressed = true;
		setDealButton(buttonPressed);
		// }
	}
	else if (GameState::WAR == state)
	{
		SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
		thrownCardsRender();
		player1Render();
		player2Render();
		player3Render();
		cardsCountMessage();
		warMessage();
	}
	else if (GameState::OVER == state)
	{
	}
	if (showStats)
	{
		statsMessage(players);
	}

	SDL_RenderPresent(renderer);
}

void App::loadTextureOnDeck()
{
	const char *const arrSuit[4] = {"clubs", "diamonds", "hearts", "spades"};
	const char *const arrFace[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"};
	for (int i = 0; i < deck->getDeck().size(); i++)
	{
		std::string filePathRoot = "/home/default/asparuh_project/assets/cards/";
		filePathRoot.append(arrFace[i % 13]).append("_of_").append(arrSuit[i % 4]).append(".png");
		SDL_Texture *tex = loadTexture(filePathRoot, renderer);
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
		case SDL_MOUSEBUTTONUP:
		{
			int msx, msy;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// Play War Round returns true and continue the game as War
				// Waiting to press a button

				SDL_GetMouseState(&msx, &msy);
				if (isClickableRectClicked(&dRectButtonStart, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonStartPlayer2, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonStartPlayer3, mouseDownX, mouseDownY, msx, msy))
				{
					if (wasStartPressed)
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
						wasStartPressed = true;
						std::cerr << " CLICKED START"
								  << "\n";
						SDL_SetTextureAlphaMod(textStartTexture, 128);
						SDL_SetTextureAlphaMod(card1Texture, 255);
						state = GameState::PLAYING;
					}
				}
				else if (isClickableRectClicked(&dRectButtonDeal, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonDealPlayer2, mouseDownX, mouseDownY, msx, msy) || isClickableRectClicked(&dRectButtonDealPlayer3, mouseDownX, mouseDownY, msx, msy))
				{
					// Having a boolean helps with indicating whether there is a war or not
					// a possible if structure here
					if (isClickableRectClicked(&dRectButtonDeal, mouseDownX, mouseDownY, msx, msy) && !getButtonPressedDeal1())
					{

						players[0].setTurn(true);

						c1 = players[0].getPlayerDeck().front();
						buttonPressed = true;
						setDealButton(buttonPressed);
						setButtonPressedDeal1(true);
						setButtonPressedDeal2(false);
					}
					else if (isClickableRectClicked(&dRectButtonDealPlayer2, mouseDownX, mouseDownY, msx, msy) && !getButtonPressedDeal2())
					{

						players[1].setTurn(true);
						c2 = players[1].getPlayerDeck().front();
						buttonPressed = true;
						setDealButton(buttonPressed);
						setButtonPressedDeal3(false);
					}
					else if (isClickableRectClicked(&dRectButtonDealPlayer3, mouseDownX, mouseDownY, msx, msy) && !getButtonPressedDeal3())
					{
						players[2].setTurn(true);

						c3 = players[2].getPlayerDeck().front();

						buttonPressed = true;
						setDealButton(buttonPressed);
						setButtonPressedDeal1(false);
						setButtonPressedDeal2(true);
						setButtonPressedDeal3(true);
					}
					if (player1->isActive() && player2->isActive() && player3->isActive())
					{
					}
					if (true == players[0].getTurn() && true == players[1].getTurn() && true == players[2].getTurn())
					{
						players[0].setTurn(false);
						players[1].setTurn(false);
						players[2].setTurn(false);
						saveStatsXML(players);
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
				else if (isClickableRectClicked(&statsButton, mouseDownX, mouseDownY, msx, msy))
				{
					showStats = !showStats;
					statsMessage(players);
				}
				else if (isClickableRectClicked(&allDeal, mouseDownX, mouseDownY, msx, msy))
				{
					players[0].setTurn(true);
					players[1].setTurn(true);
					players[2].setTurn(true);

					c1 = players[0].getPlayerDeck().front();
					c2 = players[1].getPlayerDeck().front();
					c3 = players[2].getPlayerDeck().front();

					isRoundPlayed = true;
					saveStatsXML(players);
					std::cerr << "---------------------------------\n";
					int sum = 0;
					for (unsigned i = 0; i < players.size(); i++)
					{
						sum += players[i].cntPlayerDeck();
					}
					std::cerr << "\nSUM " << sum;
					std::cerr << "\nnew  round\n";
					for (int i = 0; i < 10; i++)
					{
						playRound();
					}
					// playRound();
				}
				else if (isClickableRectClicked(&reset, mouseDownX, mouseDownY, msx, msy))
				{
					restartGame();
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
bool App::registerWinner(std::vector<Card> &deskDeck, unsigned winner)
{
	std::cerr << "Winner is: " << winner + 1 << '\n';
	std::string warMessage = "Winner is: " + std::to_string(winner);
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
int App::calcTiePlayersResizable(const Card &c, std::vector<Player> players)
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
Card App::getBiggestWarCard(Player &player1, Player &player2)
{
	Card biggest = {};
	biggest.value = 2;
	if (!player1.getPlayerDeck().empty())
	{
		biggest = player1.getPlayerDeck().front();
	}
	if (!player2.getPlayerDeck().empty() && player2.getPlayerDeck().front() > biggest)
	{
		biggest = player2.getPlayerDeck().front();
	}

	return biggest;
}
Card App::getBiggestPlayerCard()
{
	Player &player1 = players[0];
	Player &player2 = players[1];
	Player &player3 = players[2];

	Card biggest = {};
	biggest.value = 2;
	if (!players[0].getPlayerDeck().empty())
	{
		biggest = players[0].getPlayerDeck().front();
	}
	if (!players[1].getPlayerDeck().empty() && players[1].getPlayerDeck().front() > biggest)
	{
		biggest = players[1].getPlayerDeck().front();
	}
	if (!players[2].getPlayerDeck().empty() && players[2].getPlayerDeck().front() > biggest)
	{
		biggest = players[2].getPlayerDeck().front();
	}

	return biggest;
}
bool App::hasWar()
{
	Card c = getBiggestPlayerCard();
	if (calcTiePlayers(c) > 1)
	{
		return true;
	}
	return false;
}
bool App::hasWarForTwo(Player &player1, Player &player2)
{
	Card c = getBiggestWarCard(player1, player2);
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
			players[i].incrementPoint(c);
			return i;
		}
	}
	return (unsigned)players.size();
}
unsigned App::getWinner()
{
	if (hasWar())
	{
		return (unsigned)players.size(); // invalid player
	}
	Card c = getBiggestPlayerCard(); // bug: does not compare the right cards, comparing the player's decks but the cards are already pulled
	return findPlayerWithCard(c);
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
bool App::isGameOver()
{
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
		player3.setWins(1);
		return true;
	}
	else if (player1.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 2 wins!\n";
		player2.setWins(1);

		return true;
	}
	else if (player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 1 wins!\n";
		player1.setWins(1);

		return true;
	}
	if (player1.cntPlayerDeck() == 0 && player2.cntPlayerDeck() != 0 && player3.cntPlayerDeck() != 0)
	{
		player1.setLosses(1);
	}
	else if (player1.cntPlayerDeck() != 0 && player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() != 0)
	{
		player2.setLosses(1);
	}
	else if (player1.cntPlayerDeck() != 0 && player2.cntPlayerDeck() != 0 && player3.cntPlayerDeck() == 0)
	{
		player3.setLosses(1);
	}
	return false;
}

bool App::PlayWarRound()
{
	// koi players imat ednakvi karti, za da se opredeli activityto
	int playersInWarCnt = 0;
	Card strongestCardInWar;
	Card strongestCard;
	std::vector<Card> deskDeck;

	if (war) // if there is second war in the first war
	{
		strongestCardInWar = {};
		strongestCardInWar.value = 2;
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (players[i].isActive())
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() > strongestCardInWar)
				{
					strongestCardInWar = players[i].getMiniPlayerDeck().back();
				}

				// trnasfering  the miniDeck to Deck
				for (unsigned j = 0; j < players[i].getMiniPlayerDeck().size(); j++)
				{
					deskDeck.push_back(players[i].getMiniPlayerDeck()[j]);
				}
			}
		}

		for (unsigned i = 0; i < players.size(); i++)
		{
			if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == strongestCardInWar)
			{
				playersInWarCnt++;
			}
		}
		war = false;
	}
	else // for the first round of war
	{
		strongestCard = getBiggestPlayerCard();
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
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == strongestCard)
		{
			players[i].setActive(true);
		}
	}

	// checking if the cards are the strongest in player mini deck
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() == strongestCardInWar)
		{
			players[i].setActive(true);
		}
	}

	// pulling the card which triggered the state
	for (unsigned i = 0; i < players.size() && !players[i].getPlayerDeck().empty(); i++)
	{
		deskDeck.push_back(players[i].pullCard());
	}

	std::cerr << "=======================\n";
	std::cerr << "\tDesk Deck\n";
	printDeck(deskDeck);
	std::cerr << "\tPlayers:\n";
	for (unsigned i = 0; i < players.size(); i++)
	{
		printPlayer(i);
	}

	// Every player throws 3 cards
	for (size_t i = 0; i < players.size(); i++)
	{
		std::cerr << std::boolalpha << players[i].isActive() << '\n';
	}

	for (unsigned i = 0; i < players.size(); i++)
	{
		if (players[i].isActive())
		{
			for (unsigned j = 0; j < 3; j++) // throwing 3 cards from each
			{
				if (players[i].getPlayerDeck().empty())
					continue;
				Card temp = players[i].getPlayerDeck().front();
				players[i].getMiniPlayerDeck().push_back(temp);
				Card pC = players[i].pullCard();
				deskDeck.push_back(pC);
			}
		}
	}

	if (players[0].isActive() && players[1].isActive())
	{
		std::cerr << "Here we are in player 1 and 2 isActive\n";
		// printing
		std::cerr << "Player 1 is playing:\n";
		printDeck(players[0].getMiniPlayerDeck());

		std::cerr << "Player 2 is playing:\n";
		printDeck(players[1].getMiniPlayerDeck());

		if (players[0].getMiniPlayerDeck().empty() || players[1].getMiniPlayerDeck().empty())
		{
			if (players[0].getMiniPlayerDeck().empty())
			{
				registerWinner(deskDeck, 0);
				return true;
			}
			else if (players[1].getMiniPlayerDeck().empty())
			{
				registerWinner(deskDeck, 1);
				return true;
			}
		}
		if (players[0].getMiniPlayerDeck().back() != players[1].getMiniPlayerDeck().back())
		{
			Card biggest = {};
			biggest.value = 2;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() > biggest)
				{
					biggest = players[i].getMiniPlayerDeck().back();
				}
			}

			int winner = -1;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() == biggest)
				{
					players[i].incrementPoint(biggest);
					winner = i;
				}
			}

			registerWinner(deskDeck, winner);

			// for (unsigned i = 0; i < players.size(); i++)
			// {
			// 	players[i].getMiniPlayerDeck().clear();
			// 	players[i].setActive(false);
			// }

			return true; // the war is done
		}
		else
		{
			std::cout << "\n\tWAR AGAIN\n";
			war = true;
			PlayWarRound();
			// War again
		}
	}
	else if (players[0].isActive() && players[2].isActive())
	{
		std::cerr << "here are playing player1 and player 3\n";
		std::cerr << "Player 1 is playing:\n";
		printDeck(players[0].getMiniPlayerDeck());

		std::cerr << "Player 3 is playing:\n";
		printDeck(players[2].getMiniPlayerDeck());
		if (players[0].getMiniPlayerDeck().empty() || players[2].getMiniPlayerDeck().empty())
		{
			if (players[0].getMiniPlayerDeck().empty())
			{
				registerWinner(deskDeck, 0);

				return true;
			}
			else if (players[2].getMiniPlayerDeck().empty())
			{
				registerWinner(deskDeck, 2);
				return true;
			}
		}
		if (players[0].getMiniPlayerDeck().back() != players[2].getMiniPlayerDeck().back())
		{
			Card biggest = {};
			biggest.value = 2;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() > biggest)
				{
					biggest = players[i].getMiniPlayerDeck().back();
				}
			}

			int winner = -1;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() == biggest)
				{
					players[i].incrementPoint(biggest);
					winner = i;
				}
			}

			std::cerr << "\n";
			registerWinner(deskDeck, winner);
			// for (unsigned i = 0; i < players.size(); i++)
			// {
			// 	players[i].getMiniPlayerDeck().clear();
			// 	players[i].setActive(false);
			// }

			return true; // the war is done
		}
		else
		{
			std::cout << "\n\tWAR AGAIN\n";
			war = true;
			PlayWarRound();

			// War again
		}
	}
	else if (players[1].isActive() && players[2].isActive())
	{
		std::cerr << "Here are playing player 2 and 3\n";
		std::cerr << "Player 2 is playing:\n";
		printDeck(players[1].getMiniPlayerDeck());

		std::cerr << "Player 3 is playing:\n";
		printDeck(players[2].getMiniPlayerDeck());
		if (players[1].getMiniPlayerDeck().empty() || players[2].getMiniPlayerDeck().empty())
		{
			if (players[1].getMiniPlayerDeck().empty())
			{
				registerWinner(deskDeck, 1);

				return true;
			}
			else if (players[2].getMiniPlayerDeck().empty())
			{
				registerWinner(deskDeck, 2);
				return true;
			}
		}
		if (players[1].getMiniPlayerDeck().back() != players[2].getMiniPlayerDeck().back())
		{
			Card biggest = {};
			biggest.value = 2;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() > biggest)
				{
					biggest = players[i].getMiniPlayerDeck().back();
				}
			}

			int winner = -1;
			for (unsigned i = 0; i < players.size(); i++)
			{
				if (!players[i].getMiniPlayerDeck().empty() && players[i].getMiniPlayerDeck().back() == biggest)
				{
					players[i].incrementPoint(biggest);
					winner = i;
				}
			}
			std::cerr << "\n";
			registerWinner(deskDeck, winner);

			// for (unsigned i = 0; i < players.size(); i++)
			// {
			// 	players[i].getMiniPlayerDeck().clear();
			// 	players[i].setActive(false);
			// }

			return true; // the war is done
		}
		else
		{
			std::cout << "\n\tWAR AGAIN\n";
			war = true;
			PlayWarRound();

			// WAR again
		}
	}
	else
	{
		std::cout << "\n\tERROR\n";

		return false;
	}

	// tova go povtarqme dokato edna ot kartite e po golqma
	// pehrylqme kartite na popeditelq
	// predvijdame kyde restartiterame activitito
	// for (unsigned i = 0; i < players.size(); i++)
	// {
	// 	players[i].getMiniPlayerDeck().clear();
	// 	players[i].setActive(false);
	// }

	return true;
}
void App::playRound()
{
	if (isGameOver())
	{
		return;
	}
	round++;

	if (hasWar())
	{
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
}

unsigned App::playDuoNormalRound(Player &player1, Player &player2, std::vector<Card> &deskDeck)
{
	if (player1.getPlayerDeck().empty())
	{
		std::cerr << "Empty duoNormal\n";
		return 2;
	}
	else if (player2.getPlayerDeck().empty())
	{
		std::cerr << "Empty duoNormal\n";
		return 1;
	}
	Card biggest = getBiggestWarCard(player1, player2);
	int tiePlayers = calcTiePlayers(biggest);
	std::cerr << '\n';
	if (tiePlayers >= 2)
	{
		deskDeck.push_back(player1.pullCard());
		deskDeck.push_back(player2.pullCard());

		return playDuoNormalRound(player1, player2, deskDeck);
	}
	else
	{

		if (player1.getPlayerDeck().front() == biggest && player2.getPlayerDeck().empty())
		{
			deskDeck.push_back(player1.pullCard());
			return 1;
		}
		else if (player2.getPlayerDeck().front() == biggest && player1.getPlayerDeck().empty())
		{
			deskDeck.push_back(player2.pullCard());
			return 2;
		}
		else if (player1.getPlayerDeck().front() == biggest)
		{
			deskDeck.push_back(player1.pullCard());
			deskDeck.push_back(player2.pullCard());
			return 1;
		}
		else if (player2.getPlayerDeck().front() == biggest)
		{
			deskDeck.push_back(player1.pullCard());
			deskDeck.push_back(player2.pullCard());
			return 2;
		}

		return -1;
	}
	return -1;
}

void App::printPlayer(unsigned i)
{
	std::cerr << "Player " << (i + 1) << ", Cards number: " << players[i].cntPlayerDeck() << '\n';
	std::cerr << "Cards:" << '\n';
	players[i].printCards();
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
	buttonPressedDeal[0] = pr;
}
void App::setButtonPressedDeal2(bool pr)
{
	buttonPressedDeal[1] = pr;
}
void App::setButtonPressedDeal3(bool pr)
{
	buttonPressedDeal[2] = pr;
}
bool App::getButtonPressedDeal1() const
{
	return buttonPressedDeal[0];
}
bool App::getButtonPressedDeal2() const
{
	return buttonPressedDeal[1];
}
bool App::getButtonPressedDeal3() const
{
	return buttonPressedDeal[2];
}
bool App::getDealButton() const
{
	return buttonPressed;
}
// Player 1 Cards
void App::player1Render()
{
	SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);

	int numCards = players[0].cntPlayerDeck();

	SDL_Rect dRectPlayer1Cards[PLAYERS_RENDERING_CARDS_CNT] = {};

	for (int i = 0; i < numCards; i++)
	{
		dRectPlayer1Cards[i] = {0, 150 + i * 2, tw, th};
		SDL_RenderCopy(renderer, card1Texture, nullptr, &dRectPlayer1Cards[i]);
	}
	// if players 0 and 1 or 0 and 2 are active its always activeplayer[0]
	if (state == GameState::WAR && players[0].isActive())
	{
		SDL_Rect dRectPlayer1MiniCards[9] = {};

		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		for (int i = 0; i < players[0].getMiniPlayerDeck().size(); i++)
		{
			dRectPlayer1MiniCards[i] = {200, 120 + i * 20, tw, th};
			SDL_RenderCopy(renderer, players[0].getMiniPlayerDeck()[i].texture, nullptr, &dRectPlayer1MiniCards[i]);
		}
	}

	// players[0].setActive(false);
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
	SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
	int numCards = players[1].cntPlayerDeck();
	SDL_Rect dRectPlayer2Cards[PLAYERS_RENDERING_CARDS_CNT] = {};
	for (int i = 0; i < numCards; i++)
	{
		dRectPlayer2Cards[i] = {220, 400 + i * 2, tw, th};
		SDL_RenderCopy(renderer, card1Texture, nullptr, &dRectPlayer2Cards[i]);
	}
	if (state == GameState::WAR && players[1].isActive())
	{
		SDL_Rect dRectPlayer2MiniCards[9] = {};
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		for (int i = 0; i < players[1].getMiniPlayerDeck().size(); i++)
		{
			dRectPlayer2MiniCards[i] = {300, 220 + i * 20, tw, th};
			SDL_RenderCopy(renderer, players[1].getMiniPlayerDeck()[i].texture, nullptr, &dRectPlayer2MiniCards[i]);
		}
	}
	dRectButtonStartPlayer2 = {350, 450, 70, 30};
	dRectButtonDealPlayer2 = {350, 500, 70, 30};
	SDL_RenderFillRect(renderer, &dRectButtonStartPlayer2);
	SDL_RenderFillRect(renderer, &dRectButtonDealPlayer2);
	SDL_RenderCopy(renderer, textStartTexture, nullptr, &dRectTextStartPlayer2);
	SDL_RenderCopy(renderer, textDealTexture, nullptr, &dRectTextDealPlayer2);
	SDL_RenderCopy(renderer, textPlayer2Texture, nullptr, &dRectTextPlayer2);
}

// Player 3 Cards
void App::player3Render()
{
	// player 3 render in war of 3
	SDL_QueryTexture(card1Texture, 0, 0, &tw, &th);
	int numCards = players[2].cntPlayerDeck();
	SDL_Rect dRectPlayer3Cards[PLAYERS_RENDERING_CARDS_CNT] = {};
	for (int i = 0; i < numCards; i++)
	{
		dRectPlayer3Cards[i] = {800 - tw, 150 + i * 2, tw, th};
		SDL_RenderCopy(renderer, card1Texture, nullptr, &dRectPlayer3Cards[i]);
	}
	if (state == GameState::WAR && players[2].isActive())
	{
		// std::cerr << std::boolalpha << players[2].isActive();
		SDL_Rect dRectPlayer3MiniCards[9] = {};
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		for (int i = 0; i < players[2].getMiniPlayerDeck().size(); i++)
		{
			dRectPlayer3MiniCards[i] = {400, 120 + i * 20, tw, th};
			SDL_RenderCopy(renderer, players[2].getMiniPlayerDeck()[i].texture, nullptr, &dRectPlayer3MiniCards[i]);
		}
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
	if (players[0].cntPlayerDeck() == 0)
	{
		SDL_Rect dRectCard2Thrown;
		SDL_Rect dRectCard3Thrown;
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		dRectCard2Thrown = {300, 200, tw, th};
		dRectCard3Thrown = {400, 100, tw, th};
		SDL_RenderCopy(renderer, c2.texture, nullptr, &dRectCard2Thrown);
		SDL_RenderCopy(renderer, c3.texture, nullptr, &dRectCard3Thrown);
		return;
	}
	else if (players[1].cntPlayerDeck() == 0)
	{
		SDL_Rect dRectCard1Thrown;
		SDL_Rect dRectCard3Thrown;
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		dRectCard1Thrown = {200, 100, tw, th};
		dRectCard3Thrown = {400, 100, tw, th};
		SDL_RenderCopy(renderer, c1.texture, nullptr, &dRectCard1Thrown);
		SDL_RenderCopy(renderer, c3.texture, nullptr, &dRectCard3Thrown);
		return;
	}
	else if (players[2].cntPlayerDeck() == 0)
	{
		SDL_Rect dRectCard1Thrown;
		SDL_Rect dRectCard2Thrown;
		SDL_QueryTexture(c1.texture, 0, 0, &tw, &th);
		dRectCard1Thrown = {200, 100, tw, th};
		dRectCard2Thrown = {300, 200, tw, th};
		SDL_RenderCopy(renderer, c1.texture, nullptr, &dRectCard1Thrown);
		SDL_RenderCopy(renderer, c2.texture, nullptr, &dRectCard2Thrown);
		return;
	}
	SDL_Rect dRectCard1Thrown;
	SDL_Rect dRectCard2Thrown;
	SDL_Rect dRectCard3Thrown;
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
	Message2Pos.x = 360;
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

// void App::winnerMessage()
// {
// 	std::string winnerMessage = "P" + std::to_string(winner);
// 	SDL_Color textColor = {255, 255, 255, 255};
// 	SDL_Surface *winnerSurface = TTF_RenderText_Solid(font, winnerMessage.c_str(), textColor);
// 	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, winnerSurface);

// 	SDL_Rect warMessagePos;
// 	warMessagePos.x = 550;
// 	warMessagePos.y = 475;
// 	warMessagePos.h = 40;
// 	warMessagePos.w = 80;

// 	SDL_RenderCopy(renderer, Message, NULL, &warMessagePos);
// 	SDL_DestroyTexture(Message);
// 	SDL_FreeSurface(winnerSurface);

// }

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
void App::statsMessage(std::vector<Player> &players)
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
	statsPos.w = 750;
	statsPos.h = 50;

	SDL_RenderCopy(renderer, Message, NULL, &statsPos);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(statsSurface);
}

void App::updateStatsXML(std::vector<Player> &players)
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

void App::saveStatsXML(std::vector<Player> &players)
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
	delete deck;
	delete player1;
	delete player2;
	delete player3;

	players.clear();
	buttonPressedDeal.clear();

	deck = new Deck();

	player1 = new Player();
	player2 = new Player();
	player3 = new Player();

	players.push_back(*player1);
	players.push_back(*player2);
	players.push_back(*player3);

	for (unsigned i = 0; i < players.size(); i++)
	{
		players[i].setActive(false);
	}

	wasStartPressed = false;
	initDeck();

	buttonPressedDeal1 = false;
	buttonPressedDeal2 = true;
	buttonPressedDeal3 = true;

	buttonPressedDeal.push_back(buttonPressedDeal1);
	buttonPressedDeal.push_back(buttonPressedDeal2);
	buttonPressedDeal.push_back(buttonPressedDeal3);

	state = GameState::START;
	buttonPressed = false;
	showStats = false;
	isRoundPlayed = false;
	SDL_SetTextureAlphaMod(textStartTexture, 255);
	updateStatsXML(players);
}

void App::setWar(bool war)
{
	war = war;
}

bool App::getWar() const
{
	return war;
}
