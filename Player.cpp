#include "Player.h"
#include <iostream>
Player::Player()
{
	this->turn = false;
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

int Player::getPoints() const
{
    return points;
}


void Player::setPoints(int pp)
{
	points = pp;
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

bool Player::getTurn() const
{
    return turn;
}

void Player::setTurn(bool turn)
{
	this->turn = turn;
}

std::vector<Card>& Player::getPlayerDeck() // ERROR: doesnt show
{
	return playerDeck;
}

size_t Player::cntPlayerDeck()
{
	// std::cerr << playerDeck.size();
	return playerDeck.size(); // IT doesnt WORK
}

void Player::dealCards(Deck& deck)
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
		case Face::Two: std::cout << "2"; break;
		case Face::Three: std::cout << "3"; break;
		case Face::Four: std::cout << "4"; break;
		case Face::Five: std::cout << "5"; break;
		case Face::Six: std::cout << "6"; break;
		case Face::Seven: std::cout << "7"; break;
		case Face::Eight: std::cout << "8"; break;
		case Face::Nine: std::cout << "9"; break;
		case Face::Ten: std::cout << "T"; break;
		case Face::Jack: std::cout << "J"; break;
		case Face::Queen: std::cout << "Q"; break;
		case Face::King: std::cout << "K"; break;
		case Face::Ace: std::cout << "A"; break;
		default:
			break;
		}
		std::cout << " Suit: ";
		switch (playerDeck[i].suit)
{
		case Suit::Clubs: std::cout << "Clubs"; break;
		case Suit::Diamonds: std::cout << "Diamonds"; break;
		case Suit::Hearts: std::cout << "Hearths"; break;
		case Suit::Spades: std::cout << "Spades"; break;
		default:
			break;
		}
		std::cout << '\n';
	}
}

void Player::incrementPoint()
    {
	points++;
    }

void Player::decrementPoint()
{
	points--;
}

Card Player::getCurrentCard() const
{
    if (!playerDeck.empty()) {
        return playerDeck.front(); 
    }
    return Card(); 
}