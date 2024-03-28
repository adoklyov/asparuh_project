#include "Player.h"
#include <iostream>
Player::Player()
{
	this->turn = false;
	this->active = true;
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

//Separate cards for war
Card Player::pullWarCard()
{
        if (!playerDeck.empty()) {
        Card card = playerDeck.front();
        playerDeck.erase(playerDeck.begin());
        return card;
    }
    return {}; 
}

short Player::getPoints() const
{
	return this->points;
}

void Player::setPoints(short pp)
{
	this->points = pp;
}

bool Player::getTurn() const
{
	return turn;
}

void Player::setTurn(bool turn)
{
	this->turn = turn;
}

std::vector<Card> &Player::getPlayerDeck()
{
	return playerDeck;
}

size_t Player::cntPlayerDeck()
{
	return playerDeck.size();
}

int Player::getWins() const
{
	return wins;
}

void Player::setWins(int w)
{
	wins = w;
}

int Player::getLosses() const
{
	return losses;
}

void Player::setLosses(int l)
{
	losses = l;
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
{
	Card c1;
	c1.value = 10;
	Card c2;
	c2.value = 10;
	Card c3;
	c3.value = 10;
	Card c4;
	c4.value = 10;
	Card c5;
	c5.value = 10;
	Card c6;
	c6.value = 10;
	Card c7;
	c7.value = 10;
	Card c8;
	c8.value = 10;
	Card c9;
	c9.value = 10;
	Card c10;
	c10.value = 10;

	playerDeck.push_back(c1);
	playerDeck.push_back(c2);
	playerDeck.push_back(c3);
	playerDeck.push_back(c4);
	playerDeck.push_back(c5);
	playerDeck.push_back(c6);
	playerDeck.push_back(c7);
	playerDeck.push_back(c8);
	playerDeck.push_back(c9);
	playerDeck.push_back(c10);
}

void Player::setCard()
{
	Card c1;
	c1.value = 10;
	c1.face = Face::Ace;
	c1.suit = Suit::Hearts;
	playerDeck.insert(playerDeck.begin() + 1, c1);
}

void Player::setActive(bool isActive)
{
	this->active = isActive;
}

bool Player::isActive()
{
    return active;
}
