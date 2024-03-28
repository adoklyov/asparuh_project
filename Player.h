#pragma once
#include <vector>
#include "Deck.h"

class Player
{
public:
	Player();
	std::vector<Card> &getPlayerDeck();
	size_t cntPlayerDeck();

	// stats
	short getPoints() const;
	int getWins() const;
	int getLosses() const;
	void setWins(int w);
	void setLosses(int l);
	bool getTurn() const;
	void setTurn(bool turn);
	void setPoints(short pp);
	Card pullCard();
	Card pullWarCard();
	void dealCards(Deck &deck);
	void printCards() const;
	void incrementPoint(const Card &c);
	void decrementPoint();
	Card getCurrentCard() const;
	void setPlayerDeck();
	void setCard();

	void setActive(bool isActive);
	bool isActive();

private:
	bool active;
	short points = 0;
	int wins = 0;
	int losses = 0;
	bool turn;
	std::vector<Card> playerDeck;
};