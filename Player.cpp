#include "Player.h"
#include <iostream>
Player::Player()
{
}

Card Player::pullCard()
{
	if (!playerDeck.empty())
	{
		Card card = playerDeck.front();
		playerDeck.erase(playerDeck.begin());
		return card;
	}
	std::cout << "No more cards!\n";
	return {};
}

unsigned int Player::getPoints() const
{
	return this->points;
}

void Player::setPoints(unsigned int points)
{
	this->points = points;
}

bool Player::getTurn() const
{
	return turn;
}

void Player::setTurn(bool turn)
{
	this->turn = turn;
}

// std::vector<Card> Player::getPlayerDeck()
// {
// 	return this->playerDeck;
// }
std::vector<Card> &Player::getPlayerDeck()
{
	return playerDeck;
}

std::vector<Card> &Player::getMiniPlayerDeck()
{
	return miniPlayerDeck;
}

size_t Player::cntPlayerDeck()
{
	return playerDeck.size();
}

int Player::getWins() const
{
	return this->wins;
}

void Player::incrementWins()
{
	this->wins++;
}

bool Player::getIsDefeated() const
{
	return this->isDefeated;
}

void Player::setIsDefeated(bool isDef)
{
	this->isDefeated = isDef;
}

int Player::getLosses() const
{
	return this->losses;
}

void Player::setWins(int wins)
{
	this->wins = wins;
}
void Player::setLosses(int losses)
{
	this->losses = losses;
}
void Player::incrementLosses()
{
	this->losses++;
}

void Player::dealCards(Deck &deck)
{
	playerDeck.clear();
	for (unsigned count = 0; count < 10; count++) 
	{
		playerDeck.push_back(deck.DealCard());
	}
}

void Player::printCards() const
{
	for (unsigned i = 0; i < playerDeck.size(); i++)
	{
		std::cout << "Face: ";
		switch (playerDeck[i].face)
		{
		case Face::Two:
			std::cout << "2";
			break;
		case Face::Three:
			std::cout << "3";
			break;
		case Face::Four:
			std::cout << "4";
			break;
		case Face::Five:
			std::cout << "5";
			break;
		case Face::Six:
			std::cout << "6";
			break;
		case Face::Seven:
			std::cout << "7";
			break;
		case Face::Eight:
			std::cout << "8";
			break;
		case Face::Nine:
			std::cout << "9";
			break;
		case Face::Ten:
			std::cout << "T";
			break;
		case Face::Jack:
			std::cout << "J";
			break;
		case Face::Queen:
			std::cout << "Q";
			break;
		case Face::King:
			std::cout << "K";
			break;
		case Face::Ace:
			std::cout << "A";
			break;
		default:
			break;
		}
		std::cout << " Suit: ";
		switch (playerDeck[i].suit)
		{
		case Suit::Clubs:
			std::cout << "Clubs";
			break;
		case Suit::Diamonds:
			std::cout << "Diamonds";
			break;
		case Suit::Hearts:
			std::cout << "Hearts";
			break;
		case Suit::Spades:
			std::cout << "Spades";
			break;
		default:
			break;
		}
		std::cout << '\n';
	}
}

void Player::incrementPoint(const Card &c)
{
	this->points += c.value;
}

void Player::decrementPoint()
{
	this->points--;
}

Card Player::getCurrentCard() const
{
	if (!playerDeck.empty())
	{
		return playerDeck.front();
	}
	return Card();
}

void Player::setPlayerDeck()
{	std::string arrSuit[4] = {"clubs", "diamonds", "hearts", "spades"};
	std::string arrFace[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"};

	Card c1;
	c1.value = 10;

	playerDeck.push_back(c1);
	for (unsigned i = 1; i < 10; i++)
	{
		Card c;
		c.value = rand()%10;
		playerDeck.push_back(c);

	}
}

void Player::setCard()
{
	Card c1 ( Suit::Hearts, Face::Ace, 14);
	playerDeck.insert(playerDeck.begin(), c1);
}


void Player::setActive(bool isActive)
{
	this->active = isActive;
}

bool Player::isActive() const
{
	return active;
}
