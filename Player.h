#pragma once
#include <vector>
#include "Deck.h"

class Player
{
public:
	Player();
	std::vector<Card> &getPlayerDeck();
	std::vector<Card>& getMiniPlayerDeck();
	size_t cntPlayerDeck();

	// stats
	unsigned int getPoints() const;
	int getWins() const;
	int getLosses() const;
	void setWins(int w);
	void setLosses(int l);
	bool getTurn() const;
	void setTurn(bool turn);
	void setPoints(unsigned int pp);
	Card pullCard();
	void dealCards(Deck &deck);
	void printCards() const;
	void incrementPoint(const Card &c);
	void decrementPoint();
	Card getCurrentCard() const;
	void setPlayerDeck();
	void setCard();
	void setFlag(bool f);
	bool getFlag() const;

	void setActive(bool isActive);
	bool isActive() const;

private:
	bool active = false;
	bool flag = false;
	unsigned int points = 0;
	int wins = 0;
	int losses = 0;
	bool turn;
	std::vector<Card> miniPlayerDeck;
	std::vector<Card> playerDeck;
};