#pragma once
#include <vector>
#include <iostream>
class SDL_Texture;
const unsigned short DECK_CAPACITY = 52;

enum Suit
{
	Clubs,
	Diamonds,
	Hearts,
	Spades
};

enum Face
{
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace
};

struct Card
{
	Suit suit = {};
	Face face = {};
	unsigned short value = 0;
	SDL_Texture* texture = nullptr;

	void print() 
	{
		std::cerr << suit << ' ' << face << ' ' << value << ' '; 
	}
};

class Deck
{
public:
	Deck();
	void print() const;
	Card DealCard();
	void riffleShuffle();
	void shuffle();

	Card DealCardForManager();


    std::vector<Card>& getDeck();
private:
	std::vector<Card> deck;
};