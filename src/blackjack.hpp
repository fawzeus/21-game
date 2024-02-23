#include <SFML/Graphics.hpp>
#include <algorithm> //std::shuffle
#include <chrono>	 // std::chrono::system_clock
#include <iostream>
#include <map>
#include <random> // std::default_random_engine
#include <string>
#include <vector>
using namespace std;

string suits[4] = { "Hearts", "Diamond", "Spades", "Clubs" };
string ranks[13] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };
map<string, int> values = { { "Two", 2 }, { "Three", 3 }, { "Four", 4 }, { "Five", 5 }, { "Six", 6 }, { "Seven", 7 }, { "Eight", 8 }, { "Nine", 9 }, { "Ten", 10 }, { "Jack", 10 }, { "Queen", 10 }, { "King", 10 }, { "Ace", 11 } };

/***********************************************************************************************************************************/
class Card
{
private:
	string suit;
	string rank;

public:
	Card(string suit, string rank)
	{
		this->suit = suit;
		this->rank = rank;
	}

	string get_rank()
	{
		return rank;
	}
	string get_suit()
	{
		return suit;
	}
};

class Deck
{

private:
	vector<Card> lis;

public:
	Deck()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				Card c(suits[i], ranks[j]);
				lis.push_back(c);
			}
		}
	}

	void shuffle_Deck()
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(lis.begin(), lis.end(), default_random_engine(seed));
	}
	Card pull_a_card()
	{
		auto c = lis.back();
		lis.pop_back();
		return c;
	}
};

class Hand
{
private:
	vector<Card> v;
	int value;
	int aces; //keep track of aces

public:
	Hand()
	{
		value = 0;
		aces = 0;
	}
	void add_card(Card c)
	{
		v.push_back(c);
		value += values[c.get_rank()];
		if (c.get_rank() == "Ace")
		{
			aces++;
		}
	}
	void adjust_for_ace() // decide the value of given ace (1 or 11)
	{
		while (value > 21 and aces != 0)
		{
			value -= 10;
			aces -= 1;
		}
	}
	vector<Card> get_v()
	{
		return v;
	}
	int get_value()
	{
		return value;
	}
};

class Bet
{

private:
	int total;
	int current_bet;

public:
	Bet(int total = 100)
	{
		this->total = total;
		this->current_bet = 0;
	}

	int get_total()
	{
		return total;
	}

	int get_bet_current_bet()
	{
		return current_bet;
	}

	void win()
	{
		total += current_bet;
	}
	void lose()
	{
		total -= current_bet;
	}
	void set_ships(int sh)
	{
		current_bet = sh;
	}

	/*void donner_bet_en_Game()
	{

		string str;
		int n;
		while (1)
		{
			try
			{
				cout << "How many chips would you like to bet? " << endl;
				cin >> str;
				n = stoi(str);
				if (n > total)
				{
					cout << "Your bet can't exceed " << total << endl;
					continue;
				}
				if (n <= 0)
				{
					cout << "Your bet must be > 0 " << endl;
					continue;
				}
				else
				{
					break;
				}
			}
			catch (...)
			{
				cout << "Sorry! Please can you type in a number: \n";
				continue;
			}
		}
		current_bet = n;
	} */
};

class Game
{
private:
	Deck d;
	Hand player_hand;
	Hand dealer_hand;
	Bet b;
	bool playing = true;

public:
	Game()
	{
		cout << "Game Started !" << endl;
	}

	void start_game(int ship)
	{
		d.shuffle_Deck();
		player_hand.add_card(d.pull_a_card());
		player_hand.add_card(d.pull_a_card());
		player_hand.adjust_for_ace();
		dealer_hand.add_card(d.pull_a_card());
		dealer_hand.add_card(d.pull_a_card());
		dealer_hand.adjust_for_ace();
		b.set_ships(ship);
	}

	void show_some_cards(sf::RenderWindow& window)
	{
		vector<Card> dealer, player;
		dealer = dealer_hand.get_v();
		player = player_hand.get_v();

		sf::Texture hiden;
		sf::Sprite hiden_spirite;
		hiden.loadFromFile("images/cardBack.png");
		hiden_spirite.setTexture(hiden);
		hiden_spirite.setPosition(300, 30);
		window.draw(hiden_spirite);
		sf::Texture cp1;
		cp1.loadFromFile("images/" + dealer[1].get_rank() + "_of_" + dealer[1].get_suit() + ".png");
		sf::Sprite crd1;
		crd1.setTexture(cp1);
		crd1.setPosition(500, 30);
		window.draw(crd1);

		sf::Texture text[50];
		sf::Sprite out[50];
		int distance = 200;
		if (player.size() > 5)
		{
			distance = 2 * distance / player.size();
		}
		for (unsigned i = 0; i < player.size(); i++)
		{
			text[i].loadFromFile("images/" + player[i].get_rank() + "_of_" + player[i].get_suit() + ".png");
			out[i].setTexture(text[i]);
			out[i].setPosition(300 + distance * i, 450);
			window.draw(out[i]);
		}
	}
	Hand get_player()
	{
		return player_hand;
	}
	Hand get_dealer()
	{
		return dealer_hand;
	}
	Bet get_bet()
	{
		return b;
	}
	void hit_player()
	{
		player_hand.add_card(d.pull_a_card());
		player_hand.adjust_for_ace();
	}
	void hit_dealer()
	{
		dealer_hand.add_card(d.pull_a_card());
		dealer_hand.adjust_for_ace();
	}
	void show_all_cards(sf::RenderWindow& window)
	{
		vector<Card> dealer = dealer_hand.get_v();
		vector<Card> player = player_hand.get_v();

		sf::Texture t[50];
		sf::Sprite t1[50];
		int distance = 200;
		if (dealer.size() > 5 )
		{
			distance = max(2.0 * distance / dealer.size(),30.0);
		}
		for (unsigned i = 0; i < dealer.size(); i++)
		{
			t[i].loadFromFile("images/" + dealer[i].get_rank() + "_of_" + dealer[i].get_suit() + ".png");
			//cout << dealer[i].get_rank() << "_of_" << dealer[i].get_suit() << endl;
			t1[i].setTexture(t[i]);
			t1[i].setPosition(300 + distance * i, 30);
			window.draw(t1[i]);
		}
		distance = 200;
		if (player.size() > 5)
		{
			distance = max(2.0 * distance / player.size(),30.0);

		}
		for (unsigned i = 0; i < player.size(); i++)
		{
			t[i].loadFromFile("images/" + player[i].get_rank() + "_of_" + player[i].get_suit() + ".png");
			t1[i].setTexture(t[i]);
			t1[i].setPosition(300 + distance * i, 450);
			window.draw(t1[i]);
			//cout << player[i].get_rank() << "_of_" << player[i].get_suit() << endl;
		}
	}
	void playin()
	{
		playing = true;
	}
	void stand()
	{
		playing = false;
	}
	bool getplaying()
	{
		return playing;
	}

	/*void afficher_dealer_hand()
	{
		vector<Card> dealer = dealer_hand.get_v();
		cout << "\nDealer's Hand: " << endl;
		for (int i = 0; i < dealer.size(); i++)
		{
			cout << dealer[i] << endl;
		}
		cout << "Dealer's Value =" << dealer_hand.get_value() << endl;
	}*/
	/*
	void hit_or_stand()
	{
		string ask;
		while (1)
		{
			cout << "\nWould you like to hit or stand? Please enter 'h' or 's': ";
			cin >> ask;
			transform(ask.begin(), ask.end(), ask.begin(), ::tolower); // a function from algorithm module that  convert string to lower ,
			if (ask[0] == 'h')
			{
				this->hit_player();
				this->show_some_cards();
				break;
			}
			if (ask[0] == 's')
			{
				cout << "Player stands, Dealer is playing." << endl;
				playing = false;
				break;
			}
			else
			{
				cout << "Sorry! I did not understand that! Please try awin!" << endl;
			}
		}
	}*/
	string player_busts()
	{
		b.lose();
		return "PLAYER BUSTS!";
	}

	string player_wins()
	{
		b.win();
		return "PLAYER WINS!";
	}

	string dealer_busts()
	{
		b.win();
		return "DEALER BUSTS!";
	}

	string dealer_wins()
	{
		b.lose();
		return "DEALER WINS!";
	}

	string push()
	{
		return "Its a push! Player and Dealer tie!";
	}
};
/*
void jouer()
{
	while (1)
	{
		cout << "Welcome to BlackJack!" << endl;

		//create and shuffle Deck
		Game game;
		game.start_game();

		game.show_some_cards();


		while (playing)
		{

			game.hit_or_stand();

			if (game.get_player_hand().get_value() > 21)
			{
				game.player_busts();
				break;
			}
		}

		if (game.get_player_hand().get_value() <= 21)
		{
			while (game.get_dealer_hand().get_value() < 17)
			{
				game.afficher_dealer_hand();
				cout << "\nthe dealer's value < 17 :" << endl;
				cout << "the dealer get an other Card\n"
					 << endl;
				game.hit_dealer_hand();
			}
			game.show_all_cards();
			if (game.get_dealer_hand().get_value() > 21)
			{
				game.dealer_busts();
			}
			if (game.get_player_hand().get_value() > 21)
			{
				game.player_busts();
			}
			else if (game.get_dealer_hand().get_value() > game.get_player_hand().get_value())
			{
				game.dealer_wins();
			}
			else if (game.get_dealer_hand().get_value() < game.get_player_hand().get_value())
			{
				game.player_wins();
			}
			else if (game.get_dealer_hand().get_value() == game.get_player_hand().get_value())
			{
				game.push();
			}
		}
		cout << "\nPlayer's winnings stand at " << game.get_bet().get_total() << endl;
		string new_game;
		cout << "\nWould you like to play awin? Enter 'y' or 'n': " << endl;
		cin >> new_game;
		transform(new_game.begin(), new_game.end(), new_game.begin(), ::tolower); // a function from algorithm module that  convert string to lowercase
		if (new_game[0] == 'y')
		{
			playing = true;
			continue;
		}
		else
		{
			cout << "\nThanks for playing!" << endl;
			break;
		}
	}
}
*/
