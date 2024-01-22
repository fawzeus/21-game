#include "blackjack.hpp"
#include "button.hpp"
#include "jeton.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm> // to use the std::shuffle function
#include <chrono>	 // to use the std::chrono::system_clock function
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random> // to use the std::default_random_engine function
#include <sstream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>
using namespace std;
int main()
{
	sf::RenderWindow window(sf::VideoMode(1300, 700), "Blackjack Game", sf::Style::Titlebar | sf::Style::Close);
	sf::Font arial;
	arial.loadFromFile("../font/arial.ttf");

	//setting Icon
	auto image = sf::Image{};
	if (image.loadFromFile("../images/icon.png"))
	{
		window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	}



	sf::Texture back;
	back.loadFromFile("../images/table1.jpg");
	sf::Sprite background(back);
	sf::Text text1("Welcome To blackjack Game! ", arial), text2("please select the ships you want to bet", arial);
	text1.setCharacterSize(30);
	text1.setStyle(sf::Text::Bold);
	text1.setPosition(400, 30);
	text1.setFillColor(sf::Color::Red);
	text2.setCharacterSize(30);
	text2.setStyle(sf::Text::Bold);
	text2.setPosition(350, 80);
	text2.setFillColor(sf::Color::Red);
	sf::Texture jet[4];
	sf::Sprite jets[4];
	Jeton jetons[4];
	for (int i = 0; i < 4; i++)
	{
		jet[i].loadFromFile("../images/" + to_string((int)pow(10, i)) + ".png");
		jets[i].setTexture(jet[i]);
		jetons[i] = Jeton(jets[i], (int)pow(10, i));
		jetons[i].setPosition(30, 350 + 70 * i);
	}
	int bet = 0;
	sf::Text text3("your bet is: " + to_string(bet)+" $", arial);
	text3.setCharacterSize(20);
	text3.setStyle(sf::Text::Bold);
	text3.setPosition(15, 300);
	text3.setFillColor(sf::Color::White);

	sf::Text text4("Dealer's value < 17 he must take another card", arial);
	text4.setCharacterSize(20);
	text4.setStyle(sf::Text::Bold);
	text4.setPosition(400, 280);
	text4.setFillColor(sf::Color::Red);

	sf::Texture clp, clnp;
	clp.loadFromFile("../images/clear_pressed.png");
	clnp.loadFromFile("../images/clear_notpressed.png");

	sf::Sprite clpr(clp), clnpr(clnp);
	Button clear(clnpr, clpr);
	clear.setPosition(500, 500);

	sf::Texture dp, dnp;
	dp.loadFromFile("../images/done_pressed.png");
	dnp.loadFromFile("../images/done_notpressed.png");

	sf::Sprite dpr(dp), dpnr(dnp);
	Button done(dpnr, dpr);
	done.setPosition(700, 500);

	sf::Texture htp, htnp;
	htp.loadFromFile("../images/hit_pressed.png");
	htnp.loadFromFile("../images/hit_notpressed.png");

	sf::Sprite hitpress(htp), hitnotpress(htnp);
	Button hit(hitnotpress, hitpress);
	hit.setPosition(230, 360);

	sf::Texture decktext;
	decktext.loadFromFile("../images/deck.png");
	sf::Sprite deck(decktext);
	deck.setPosition(10, 0);

	sf::Texture stdp, stdnp;
	stdp.loadFromFile("../images/stand_pressed.png");
	stdnp.loadFromFile("../images/stand_notpressed.png");

	sf::Sprite standp(stdp), standnp(stdnp);
	Button stand(standnp, standp);
	stand.setPosition(1000, 360);

	sf::SoundBuffer dea;
	dea.loadFromFile("../music/dealt.ogg");
	sf::Sound deal;
	deal.setBuffer(dea);

	sf::SoundBuffer snd;
	snd.loadFromFile("../music/soundtrack.ogg");
	sf::Sound soundtrack;
	soundtrack.setBuffer(snd);

	sf::SoundBuffer fai;
	fai.loadFromFile("../music/fail.wav");
	sf::Sound fail;
	fail.setBuffer(fai);

	sf::SoundBuffer vic;
	vic.loadFromFile("../music/victory.wav");
	sf::Sound victory;
	victory.setBuffer(vic);

	sf::SoundBuffer inv;
	inv.loadFromFile("../music/invalid.ogg");
	sf::Sound invalid;
	invalid.setBuffer(inv);

	sf::Texture tnop, tnonp;
	tnop.loadFromFile("../images/no_pressed.png");
	tnonp.loadFromFile("../images/no_notpressed.png");
	sf::Sprite snop(tnop), snonp(tnonp);

	Button no(snonp, snop);
	no.setPosition(500, 500);

	sf::Texture tyesp, tyesnp;
	tyesp.loadFromFile("../images/yes_pressed.png");
	tyesnp.loadFromFile("../images/yes_notpressed.png");
	sf::Sprite syesp(tyesp), syesnp(tyesnp);

	Button yes(syesnp, syesp);
	yes.setPosition(700, 500);

	sf::Text text5("Would you like to play again ?", arial);
	text5.setCharacterSize(45);
	text5.setStyle(sf::Text::Bold);
	text5.setPosition(360, 60);
	text5.setFillColor(sf::Color::Red);

	int wins = 0;
	int games = 0;
	int winnings = 0;
	int drows = 0;
	int loses = 0;

	sf::Text game_statics("Game statics:", arial);
	game_statics.setCharacterSize(20);
	game_statics.setStyle(sf::Text::Bold);
	game_statics.setPosition(20, 220);
	game_statics.setFillColor(sf::Color::Red);

	sf::Text games_text("", arial);
	games_text.setCharacterSize(20);
	games_text.setStyle(sf::Text::Bold);
	games_text.setPosition(20, 280);
	games_text.setFillColor(sf::Color::White);

	sf::Text winnings_text("", arial);
	winnings_text.setCharacterSize(20);
	winnings_text.setStyle(sf::Text::Bold);
	winnings_text.setPosition(20, 340);
	winnings_text.setFillColor(sf::Color::White);

	sf::Text wins_text("", arial);
	wins_text.setCharacterSize(20);
	wins_text.setStyle(sf::Text::Bold);
	wins_text.setPosition(20, 400);
	wins_text.setFillColor(sf::Color::White);

	sf::Text drows_text("", arial);
	drows_text.setCharacterSize(20);
	drows_text.setStyle(sf::Text::Bold);
	drows_text.setPosition(20, 460);
	drows_text.setFillColor(sf::Color::White);

	sf::Text loses_text("", arial);
	loses_text.setCharacterSize(20);
	loses_text.setStyle(sf::Text::Bold);
	loses_text.setPosition(20, 520);
	loses_text.setFillColor(sf::Color::White);

	while (window.isOpen())
	{
	label:
		games++;
		soundtrack.play();
		bet = 0;
		text3.setString("");
		text3.setString("your bet is: " + to_string(bet)+" $");
		bool welcome = true;
		bool started = false;
		soundtrack.play();
		Game g;
		g.playin();
		while (1)
		{
			sf::Event evnt;
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
					case sf::Event::Closed:
						window.close();
						exit(0);
						break;

					case sf::Event::MouseButtonPressed:
						for (int j = 0; j < 4; j++)
						{
							if (welcome && jetons[j].isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
							{
								deal.play();
								jetons[0].press();
								bet += jetons[j].getvalue();
								text3.setString("");
								text3.setString("your bet is: " + to_string(bet)+" $");
							}
						}
						if (welcome && clear.isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							deal.play();
							clear.press();
							bet = 0;
							text3.setString("");
							text3.setString("your bet is: " + to_string(bet));
						}
						if (welcome && done.isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{

							done.press();
							if (bet != 0)
							{
								deal.play();
								welcome = false;
								g.start_game(bet);
								started = true;
							}
							else
							{
								invalid.play();
							}
						}
						if (started && hit.isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							deal.play();
							hit.press();
							g.hit_player();
						}
						if (started && stand.isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							deal.play();
							stand.press();
							g.stand();
						}

						break;
					case sf::Event::MouseButtonReleased:
						for (int j = 0; j < 4; j++)
						{
							jetons[j].release();
						}
						clear.release();
						done.release();
						hit.release();
						stand.release();
						break;
					default:

						break;
				}
			}

			window.clear();
			window.draw(background);
			if (welcome)
			{
				window.draw(text1);
				window.draw(text2);
				if (clear.ispressed())
				{
					clear.drawPressedTo(window);
				}
				else
				{
					clear.drawNotPressedTo(window);
				}
				if (done.ispressed())
				{
					done.drawPressedTo(window);
				}
				else
				{
					done.drawNotPressedTo(window);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				jetons[i].drawTo(window);
			}
			window.draw(text3);
			if (started)
			{
				window.draw(deck);
				g.show_some_cards(window);
				if (hit.ispressed())
				{
					hit.drawPressedTo(window);
				}
				else
				{
					hit.drawNotPressedTo(window);
				}
				if (stand.ispressed())
				{
					stand.drawPressedTo(window);
				}
				else
				{
					stand.drawNotPressedTo(window);
				}
			}

			if (!g.getplaying() || g.get_player().get_value() > 21)
			{

				g.stand();
				string msg = "";
				if (g.get_player().get_value() <= 21)
				{
					while (g.get_dealer().get_value() < 17)
					{
						window.draw(background);
						window.draw(deck);
						for (int i = 0; i < 4; i++)
						{
							jetons[i].drawTo(window);
						}
						window.draw(text3);
						g.show_all_cards(window);
						cout << "\nthe dealer's value < 17 :" << endl;
						cout << "the dealer get an other card\n\n";
						g.hit_dealer();
						window.draw(text4);
						window.display();
						sleep(2.5);
					}
					soundtrack.pause();
					window.draw(background);
					window.draw(deck);
					for (int i = 0; i < 4; i++)
					{
						jetons[i].drawTo(window);
					}
					window.draw(text3);
					g.show_all_cards(window);
					if (g.get_dealer().get_value() > 21)
					{
						msg = g.dealer_busts();
						victory.play();
						wins++;
						winnings += bet;
					}
					else if (g.get_dealer().get_value() > g.get_player().get_value())
					{
						msg = g.dealer_wins();
						fail.play();
						winnings -= bet;
						loses++;
					}
					else if (g.get_dealer().get_value() < g.get_player().get_value())
					{
						msg = g.player_wins();
						victory.play();
						winnings += bet;
						wins++;
					}
					else if (g.get_dealer().get_value() == g.get_player().get_value())
					{
						msg = g.push();
						victory.play();
						drows++;
					}
				}
				else
				{
					winnings -= bet;
					loses++;
					window.draw(background);
					window.draw(deck);
					for (int i = 0; i < 4; i++)
					{
						jetons[i].drawTo(window);
					}
					window.draw(text3);

					g.show_all_cards(window);
					msg = g.player_busts();
					soundtrack.pause();
					fail.play();
				}
				//cout << msg;
				sf::Text alert(msg, arial);
				alert.setCharacterSize(30);
				alert.setStyle(sf::Text::Bold);
				alert.setPosition(500, 345);
				alert.setFillColor(sf::Color::Red);
				window.draw(alert);
				window.display();
				sf::Event event;
				sleep(8);

				winnings_text.setString("your winnings is : " + to_string(winnings) + " $");
				double res1 = 1.0 * loses / games * 100;
				std::stringstream stream1;
				stream1 << std::fixed << std::setprecision(2) << res1;
				std::string s1 = stream1.str();
				loses_text.setString("your lose rate is : " + s1 + " %");
				double res2 = 1.0 * wins / games * 100;
				std::stringstream stream2;
				stream2 << std::fixed << std::setprecision(2) << res2;
				std::string s2 = stream2.str();
				wins_text.setString("your winning rate is : " + s2 + " %");
				std::stringstream stream3;
				double res3 = 1.0 * drows / games * 100;
				stream3 << std::fixed << std::setprecision(2) << res3;
				std::string s3 = stream3.str();
				drows_text.setString("your drows rate is : " + s3 + " %");
				string mystring = " game";
				if (games > 1)
					mystring += "s";

				games_text.setString("you 've played : " + to_string(games) + mystring);

				while (window.waitEvent(event))
				{
					window.clear();
					window.draw(background);
					window.draw(game_statics);
					window.draw(winnings_text);
					window.draw(loses_text);
					window.draw(wins_text);
					window.draw(drows_text);
					window.draw(games_text);
					window.draw(text5);
					if (event.type == sf::Event::Closed)
					{
						window.close();
						exit(0);
					}
					else if (event.type == sf::Event::MouseButtonPressed)
					{
						if (yes.isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							deal.play();
							goto label;
						}
						else if (no.isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							deal.play();
							exit(0);
						}
					}
					else if (event.type == sf::Event::MouseButtonReleased)
					{
						yes.release();
						no.release();
					}
					yes.drawNotPressedTo(window);
					no.drawNotPressedTo(window);
					window.display();
				}
			}
			window.display();
		}
	}
}
