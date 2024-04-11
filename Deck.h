#pragma once
#include <deque>
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
	Card() = default;
	Card(Suit suit, Face face, unsigned short value);
	Suit suit = {};
	Face face = {};
	unsigned short value = 2;
	SDL_Texture *texture = nullptr;

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

	std::deque<Card> &getDeck();

private:
	std::deque<Card> deck;
};