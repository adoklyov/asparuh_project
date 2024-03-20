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

	short getPoints() const;
	bool getTurn() const;
	void setTurn(bool turn);
	void setPoints(short pp);
	Card pullCard();
	void dealCards(Deck& deck);
	void printCards() const;
	void incrementPoint();
	void decrementPoint();
	Card getCurrentCard() const;
private:
	short points;
	bool turn;
	std::vector<Card> playerDeck;
};
#endif