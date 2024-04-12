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
	void setWins(int wins);
	void setLosses(int losses);
	void incrementWins();
	void incrementLosses();
	bool getTurn() const;
	void setTurn(bool turn);
	void setPoints(unsigned int points);
	Card pullCard();
	void dealCards(Deck &deck);
	void printCards() const;
	void incrementPoint(const Card &c);
	void decrementPoint();
	Card getCurrentCard() const;
	void setPlayerDeck();
	void setCard();
	void setIsDefeated(bool isDef);
	bool getIsDefeated() const;

	void setActive(bool isActive);
	bool isActive() const;

	//Turn Arrow Methods

private:

	//Turn Arrow

	bool active = false;
	//flag follows who died first, so he can get a point on losses
	bool isDefeated = false;
	unsigned int points = 0;
	int wins = 0;
	int losses = 0;
	bool turn=0;
	std::vector<Card> miniPlayerDeck;
	std::vector<Card> playerDeck;
};