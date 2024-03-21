#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "Deck.h"
///home/default/asparuh_project/assets/fonts/

class Player
{
public:
	Player();

	std::vector<Card>& getPlayerDeck();
	size_t cntPlayerDeck();
	// stats
	int getPoints() const;
	int getWins() const;
	int getLosses() const;
	void setWins(int w);
	void setLosses(int l);
	bool getTurn() const;
	void setTurn(bool turn);
	void setPoints(int pp);
	Card pullCard();
	void dealCards(Deck& deck);
	void printCards() const;
	void incrementPoint();
	void decrementPoint();
	Card getCurrentCard() const;
private:
 	int points = 0;
	int wins = 0;
	int losses = 0;
	bool turn;
	std::vector<Card> playerDeck;
};
#endif