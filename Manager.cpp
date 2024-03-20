#include "Manager.h"
#include <iostream>
#include <random>

Manager::Manager(Player& player1, Player& player2, Player& player3)
	:round(0), session(0)
{
	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);
}

bool operator>(const Card& card1, const Card& card2)
{
	return card1.value > card2.value;
}

bool operator<(const Card& card1, const Card& card2)
{
	return card1.value < card2.value;
}

bool operator==(const Card& card1, const Card& card2)
{
	return (card1.value == card2.value);
}

unsigned short Manager::getRound() const
{
	return this->round;
}

unsigned short Manager::getSession() const
{
	return this->session;
}

std::vector<Player>& Manager::getPlayers()
{
	return this->players;
}

void Manager::printPlayer(unsigned i)
{
	std::cerr << "Player " << (i + 1) << ", Cards number: " << players[i].cntPlayerDeck() << '\n';
	//std::cerr << "Cards:" << '\n';
	players[i].printCards();
}

void Manager::printDeck(std::vector<Card>& deck) const
{
	for (unsigned i = 0; i < deck.size(); i++)
	{
		std::cerr << "Face: ";
		switch (deck[i].face)
		{
		case Face::Two: std::cerr << "2"; break;
		case Face::Three: std::cerr << "3"; break;
		case Face::Four: std::cerr << "4"; break;
		case Face::Five: std::cerr << "5"; break;
		case Face::Six: std::cerr << "6"; break;
		case Face::Seven: std::cerr << "7"; break;
		case Face::Eight: std::cerr << "8"; break;
		case Face::Nine: std::cerr << "9"; break;
		case Face::Ten: std::cerr << "T"; break;
		case Face::Jack: std::cerr << "J"; break;
		case Face::Queen: std::cerr << "Q"; break;
		case Face::King: std::cerr << "K"; break;
		case Face::Ace: std::cerr << "A"; break;
		default:
			break;
		}
		std::cerr << " Suit: ";
		switch (deck[i].suit)
		{
		case Suit::Clubs: std::cerr << "Clubs"; break;
		case Suit::Diamonds: std::cerr << "Diamonds"; break;
		case Suit::Hearts: std::cerr << "Hearts"; break;
		case Suit::Spades: std::cerr << "Spades"; break;
		default:
			break;
		}
		std::cerr << '\n';
	}
}

void Manager::startMatch()
{
	std::cerr << "Starting new match !" << '\n';
	std::cerr << "Good luck!" << '\n';
	for (int i = 0; i < getPlayers().size(); i++)
	{
		printPlayer(i);
	}
}

void Manager::playRound()
{
	if (isGameOver())
	{
		return;
	}
	round++;

	if (hasWar())
	{
		playWarRound();
	}
	else
	{
		playNormalRound();
	}
}

bool Manager::isGameOver()
{
	Player& player1 = getPlayers()[0];
	Player& player2 = getPlayers()[1];
	Player& player3 = getPlayers()[2];
	if (player1.cntPlayerDeck() == 0 && player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Tie!";
		return true;

	}
	else if (player1.cntPlayerDeck() == 0 && player2.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 3 wins!";
		return true;
	}
	else if (player1.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 2 wins!";
		return true;
	}
	else if (player2.cntPlayerDeck() == 0 && player3.cntPlayerDeck() == 0)
	{
		std::cerr << "Player 1 wins!";
		return true;
	}
	return false;
}

Card Manager::getBiggestPlayerCard()
{
	Player& player1 = players[0];
	Player& player2 = players[1];
	Player& player3 = players[2];

	Card biggest = {};
	biggest.value = 2;
	if (!player1.getPlayerDeck().empty())
	{
		biggest = player1.getPlayerDeck().front();
	}
	if (!player2.getPlayerDeck().empty() && player2.getPlayerDeck().front() > biggest)
	{
		biggest = player2.getPlayerDeck().front();
	}
	if (!player3.getPlayerDeck().empty() && player3.getPlayerDeck().front() > biggest)
	{
		biggest = player3.getPlayerDeck().front();
	}

	return biggest;
}

int Manager::calcTiePlayers(const Card& c)
{
	// int count = 0;
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == c)
		{
			count++;
		}
	}
	return count;
}
unsigned Manager::findPlayerWithCard(const Card& c)
{
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (!players[i].getPlayerDeck().empty() && players[i].getPlayerDeck().front() == c)
		{
			return i;
		}
	}
	return (unsigned)players.size();
}

void Manager::playNormalRound()
{
	unsigned winner = getWinner();

	std::vector<Card> deskDeck;
	for (unsigned i = 0; i < players.size(); i++)
	{
		if (players[i].cntPlayerDeck() > 0)
		{
			Card c = players[i].pullCard();
			deskDeck.push_back(c);
		}
	}
	std::cerr << "Desk deck: " << ' ';
	printDeck(deskDeck);

	// play the game

	registerWinner(deskDeck, winner);

	for (int i = 0; i < players.size(); i++)
	{
		printPlayer(i);
	}
}

void Manager::playWarRound()
{
	std::cerr << "WAR: " << '\n';
	std::vector<bool> isActive;
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].cntPlayerDeck() > 0)
		{
			isActive.push_back(true);
		}
		else
		{
			isActive.push_back(false);
		}
	}

	std::vector<Card> deskDeck;

	while (true)
	{
		// calculate winner
		Card strongest = {};
		strongest.value = 2;
		for (int i = 0; i < players.size(); i++)
		{
			if (isActive[i] && players[i].getPlayerDeck().front() > strongest)
			{
				strongest = players[i].getPlayerDeck().front();
			}
		}

		// take cards from player
		int playersWithStrongest = 0;
		for (int i = 0; i < players.size(); i++)
		{
			if (isActive[i])
			{
				Card c = players[i].pullCard();
				deskDeck.push_back(c);
				if (c == strongest)
				{
					playersWithStrongest++;
				}
				else
				{
					isActive[i] = false;
				}
			}
		}

		if (playersWithStrongest == 1)
		{
			for (int i = 0; i < players.size(); i++)
			{
				if (isActive[i] && players[i].getPlayerDeck().front() == strongest)
				{
					// Winner
					registerWinner(deskDeck, i);
					for (int i = 0; i < players.size(); i++)
					{
						printPlayer(i);
					}
					return;
				}
			}
		}

		// is game over
		bool hasActivePlayers = false;
		for (unsigned i = 0; i < players.size(); i++)
		{
			if (isActive[i] && players[i].cntPlayerDeck() > 0)
			{
				hasActivePlayers = true;
			}
			else
			{
				isActive[i] = false;
			}
		}

		if (!hasActivePlayers)
		{
			// not specified what to do when cards over during war
			unsigned p = 0;
			while (!deskDeck.empty())
			{
				Card c = deskDeck.back();
				deskDeck.pop_back();
				players[p].getPlayerDeck().push_back(c);
				p++;
				if (p == players.size())
				{
					p = 0;
				}
			}
		}
	}
}

bool Manager::hasWar()
{
	Card c = getBiggestPlayerCard();
	if (calcTiePlayers(c) > 1)
	{
		return true;
	}
	return false;
}

unsigned Manager::getWinner()
{
	if (hasWar())
	{
		return (unsigned)players.size(); // invalid player
	}
	Card c = getBiggestPlayerCard();
	return findPlayerWithCard(c);
}

bool Manager::registerWinner(std::vector<Card>& deskDeck, unsigned winner)
{
	std::cerr << "Winner is: " << winner + 1 << '\n';
	for (unsigned i = 0; i < deskDeck.size(); i++)
	{
		players[winner].getPlayerDeck().push_back(deskDeck[i]);
	}
	return true;
}

Card Manager::getPlayerCard(unsigned int playerIndex) const {
    if (playerIndex < players.size()) {
        return players[playerIndex].getCurrentCard();
    }
    return Card(); 
}