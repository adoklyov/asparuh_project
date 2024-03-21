#include "Deck.h"
#include <iostream>
#include <random>
#include <algorithm>

Deck::Deck() : deck(DECK_CAPACITY)
{
	for (unsigned i = 0; i < DECK_CAPACITY; i++)
	{
		deck[i].suit = Suit(i % 4);
		deck[i].face = Face(i % 13);
		deck[i].value = deck[i].face + 2; // start from 2, not 0
	}
} 

Deck::~Deck()
{
}

void Deck::print() const
{
	for (unsigned i = 0; i < deck.size(); i++)
	{
		std::cout << "Face: ";
		switch (deck[i].face)
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
		switch (deck[i].suit)
		{
		case Suit::Clubs: std::cout << "Clubs"; break;
		case Suit::Diamonds: std::cout << "Diamonds"; break;
		case Suit::Hearts: std::cout << "Hearts"; break;
		case Suit::Spades: std::cout << "Spades"; break;
		default:
			break;
		}
		std::cout << " Value: " << deck[i].value << '\n';
	}
}

Card Deck::DealCardForManager()
{
	Card result = deck.front();
	deck.erase(deck.begin());
	return result;
}

Card Deck::DealCard()
{
	Card result = deck.front();
	deck.erase(deck.begin());
	return result;
}

void Deck::riffleShuffle()
{
	const unsigned halfDeckSize = DECK_CAPACITY / 2;

	Card deckFirstHalf[halfDeckSize] = {}, deckSecondHalf[halfDeckSize] = {};

	for (unsigned i = 0; i < halfDeckSize; i++)
	{
		deckFirstHalf[i] = deck[i];
	}

	for (unsigned i = halfDeckSize, k = 0; i < DECK_CAPACITY || k < halfDeckSize; i++, k++)
	{
		deckSecondHalf[k] = deck[i];
	}

	for (unsigned i = 0, j = 0; j < halfDeckSize || i < DECK_CAPACITY; i += 2, j++)
	{
		deck[i] = deckSecondHalf[j];
	}

	for (unsigned i = 1, j = 0; j < halfDeckSize || i < DECK_CAPACITY; i += 2, j++)
	{
		deck[i] = deckFirstHalf[j];
	}

}

std::vector<Card>& Deck::getDeck()
{
    return deck;
}

void Deck::shuffle()
{
	srand(unsigned(time(NULL)));
	std::random_shuffle(deck.begin(), deck.end());
}