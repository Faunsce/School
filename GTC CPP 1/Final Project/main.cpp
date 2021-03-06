﻿// Includes and Usings
#include <iostream>
using std::cout; using std::cin;
#include <string>
using std::string;
#include <fstream>
#include <filesystem>
#include "customWait.h"
#include "customIO.h"
#include "resources.h"
#include "classes.h"
#include "accManip.h"
#include "game.h"
// Variables and Data
string accPath = std::filesystem::current_path().string() + "\\accs\\";
bool playing = true;
string sInput;
char cInput;
int iInput, plrAm;
std::vector<Player> players;
int main() {
	// File check
	if (std::filesystem::is_directory(accPath)) {
		// smallBox("Success");
		// smallBox(accPath);
	} else {
		smallBox("Failure to access accs folder");
		// smallBox(accPath);
		exiting();
		return 0;
	}
	// Welcome Screen
	menuScreen(menuText);
	wait(5000);
	// Mode Select
	clearScreen();
	smallBox("What mode would you like to play? (PVP/PVE)");
	getline(cin, sInput);
	upString(sInput);
	// Input Validation
	while (sInput != "PVP" && sInput != "PVE") {
		smallBox("Invalid input, please put PVP or PVE");
		getline(cin, sInput);
		upString(sInput);
	}
	if (sInput == "PVP") {
		smallBox("How many players?");
		cin >> iInput;
		// Input Validation
		while (iInput < 2 || iInput > 4) {
			if (iInput < 2) {
				smallBox("Too few players. Please try again");
			}
			else if (iInput > 4) {
				smallBox("Too many players. Please try again");
			}
			cin.ignore(1);
			cin >> iInput;
		}
		plrAm = iInput;
	} else if (sInput == "PVE") {
		plrAm = 1;
	} else {
		smallBox("A critical error has been encountered, exiting");
		exiting();
		return 0;
	}
	// Account Select
	for (int i = 0; i < plrAm;i++) {
		bool existing;
		smallBox(" Player [" + std::to_string(i + 1) + "] : Do you already have an account? (y/n)");
		cin.ignore(1);
		cin.get(cInput);
		cInput = toupper(cInput);
		// Input Validation
		while (cInput != 'Y' && cInput != 'N') {
			smallBox("That input was invalid, please try again");
			cin.get(cInput);
			cInput = toupper(cInput);
		}
		if (cInput == 'Y') {
			std::ifstream plrAcc;
			smallBox("Please enter your account name");
			cin.ignore(1);
			getline(cin, sInput);
			plrAcc.open(accPath + sInput + "-Acc.txt");
			if (plrAcc) {
				pullAcc(plrAcc, players); 
			} else {
				smallBox("Account does not exist");
				smallBox("A critical error has been encountered, exiting");
				exiting();
				return 0;
			}
		} else if (cInput == 'N') {
			makeAcc(accPath, players);
		} else {
			smallBox("A critical error has been encountered, exiting");
			exiting();
			return 0;
		}
	}
	// Deck setup
	std::vector<Card> deck; // Make deck
	makeDeck(deck);
	// Plr's Setup
	Player dealer("", 0, 0, 0); // Create Dealer
	for (int i = 0; i < players.size(); i++) { // Setup Players
		for (int j = 0; j < 2; j++) {
			players[i].hand.push_back(Card(0, '0'));
			players[i].setBet(0);
		}
	}
	for (int i = 0; i < 2; ++i) { // Setup Dealer
		dealer.hand.push_back(Card(0, '0'));
	}
	dealer.makeDeal(true); // Dealer was normal player -> is now Dealer type of player
	// Game Loop
	while (playing) {
		wait(1);
		// Place betts
		for (int i = 0; i < players.size(); i++) {
			int tempBet;
			clearScreen();
			draw(players, dealer, i);
			smallBox("Place your bet (0 - 100)");
			cin >> tempBet;
			while (tempBet < 0 || tempBet > 100) {
				if (tempBet < 0) {
					smallBox("No negative bets, please reenter");
					cin >> tempBet;
				} else if (tempBet > 100) {
					smallBox("Bet too large, please reenter");
					cin >> tempBet;
				}
			}
			players[i].setBet(tempBet);
			players[i].setBal(players[i].getBal() - tempBet);
			clearScreen();
			draw(players, dealer, i);
		}
		// Deal Cards
		for (int i = 0; i < 2; i++) {
			// Player Draw
			for (int j = 0; j < players.size(); j++) {
				if (players[j].getBet() == 0) { // Dont deal to non betters
					continue;
				}
				players[j].hand[i] = deck.back();
				deck.pop_back();
			}
			// Dealer Draw
			dealer.hand[i] = deck.back();
			if (i == 0) {
				dealer.hand[0].hideCard(false);
			}
			deck.pop_back();
			
		}
		// Player Action
		for (int i = 0; i < players.size(); i++) {
			if (players[i].getStand() == true) {
				players[i].setStand(false);
				continue;
			}
			string sChoice;
			char cChoice;
			int iChoice;
			if (players[i].getBet() == 0) { // Skip no bet players
				continue;
			} else {
				clearScreen();
				draw(players, dealer, i);
				if (dealer.hand[1].getValue() == 1) { // Insurance
					smallBox("Would you like to take insurace? (y/n)");
					cin.ignore(1);
					cin.get(cChoice);
					cChoice = toupper(cChoice); // Input Validation
					while (cChoice != 'Y' && cChoice != 'N') {
						smallBox("Invalid input, try again");
						cin.ignore(1);
						cin.get(cChoice);
						cChoice = toupper(cChoice);
					}
					if (cChoice = 'Y') { // Get insurance amount
						smallBox("How much insuracne would you like to place?");
						cin >> iChoice;
						while (iChoice < 0 || iChoice >(players[i].getBet() / 2)) {
							if (iChoice < 0) {
								smallBox("Too small, place again");
								cin >> iChoice;
							} else if (iChoice > (players[i].getBet() / 2)) {
								smallBox("Too large, place again");
								cin >> iChoice;
							}
						}
						players[i].setInsur(iChoice);
					}
				}
				bool invalid;
				do {
					int hands = 1;
					if (players[i].getHands() == 2) {
						hands = 2;
					}
					for (int handCount = 0; handCount < hands; handCount++) {
						do {
							invalid = false;
							smallBox("What would you like to do? (Hit/Stand/Double/Split/Surrender)");
							cin.ignore(1);
							getline(cin, sChoice);
							upString(sChoice);
							while (sChoice != "HIT" && sChoice != "STAND" && sChoice != "DOUBLE" && sChoice != "SPLIT") {
								smallBox("Invalid input, try again");
								getline(cin, sChoice);
								upString(sChoice);
							}
							if (sChoice == "HIT") {
								if (deck.size() < 0) {
									smallBox("No more cards left, resetting deck");
									makeDeck(deck);
								}
								if (handCount == 0) {
									players[i].hand.push_back(deck.back());
									deck.pop_back();
								} else if (handCount == 1) {
									players[i].hand2.push_back(deck.back());
									deck.pop_back();
								}
							}
							else if (sChoice == "STAND") {
								players[i].setStand(true);
							}
							else if (sChoice == "DOUBLE") {
								if (players[i].getBal() < (players[i].getBet() * 2) || players[i].hand2.empty() != true) {
									smallBox("Not enough money/2 hands");
									invalid = true;
								}
								else {
									players[i].setBal(players[i].getBal() - players[i].getBet());
									players[i].setBet(players[i].getBal() * 2);
								}
							}
							else if (sChoice == "SPLIT") {
								if (players[i].hand[0].getValue() == players[i].hand[1].getValue() || players[i].hand2.empty() == true || players[i].hand.size() < 3) {
									smallBox("Already have 2 hands/not first 2 cards/dont have 2 of same value");
									invalid = true;
								}
								else {
									players[i].hand2[0] = players[i].hand[1];
									players[i].hand.pop_back();
									players[i].setHands(2);
									players[i].setBet(players[i].getBet());
									players[i].setBal(players[i].getBal() - players[i].getBet2());
								}
							}
							else {
								smallBox("Critical Error, exiting");
								exiting();
								return 0;
							}
							if (invalid == false) {
								clearScreen();
								draw(players, dealer, i);
							}
							if (invalid == true) {
								smallBox("You performed an invalid action, take your turn again");
							}
						} while (invalid == true);
					}
				} while (players[i].getStand() != true);
			}
		}
		// Dealer pull
		int dealerHand = 0;
		dealer.hand[0].hideCard(true);
		do {  // Determine value
			dealerHand = 0;
			for (int x = 0; x < dealer.hand.size(); x++) {
				int n = 0;
				if (dealer.hand[x].getValue() == 1) {
					int rando = rand() % 100;
					if (rando > 50) {
						dealer.hand[x].setValue(11);
					}
				}
				dealerHand += dealer.hand[x].getValue();
			}
			if (dealerHand > 16) {
				break;
			} else {
				dealer.hand.push_back(deck.back());
				dealerHand = dealerHand + dealer.hand[dealer.hand.size() - 1].getValue();
				deck.pop_back();
			}		
		} while (dealerHand < 17);
		// Game logic
		for (int i = 0; i < players.size(); i++) { // Go through all players
			int ace;
			int plrVal = 0;
			int plrVal2 = 0;
			for (int j = 0; j < players[i].hand.size(); ++j) { // Process cards for aces
				if (players[i].hand[j].getValue() == 1) {
					smallBox("Would you like this ace to be an 11 or 1?");
					cin >> ace;
					while (ace != 1 || ace != 11) {
						smallBox("Invalid input, please try again");
						cin >> ace;
					}
					draw(players, dealer, i);
					players[i].hand[j].setValue(ace);
				}
				plrVal = players[i].hand[j].getValue(); // Set player hand value
			}
			if (players[i].getHands() == 2) {
				for (int j = 0; j < players[i].hand2.size(); j++) { // Process cards for aces
					if (players[i].hand2[j].getValue() == 1) {
						smallBox("Would you like this ace to be an 11 or 1?");
						cin >> ace;
						while (ace != 1 || ace != 11) {
							smallBox("Invalid input, please try again");
							cin >> ace;
						}
						draw(players, dealer, i);
						players[i].hand2[j].setValue(ace);
					}
					plrVal = players[i].hand2[j].getValue(); // Set player hand value
				}
			}
			if (plrVal > 21) { // Check win loss
				players[i].setBet(0);
			} else if (plrVal == 21 && dealerHand == 21) {
				players[i].setBal(players[i].getBal() + players[i].getBet());
				players[i].setBet(0);
			}
			else if (plrVal == 21) {
				players[i].setBal(players[i].getBal() + (players[i].getBet() * 2.5));
				players[i].setBet(0);
			}
			else if (plrVal < 21 && plrVal > dealerHand) {
				players[i].setBal(players[i].getBal() + (players[i].getBal() * 2.5));
				players[i].setBet(0);
			}
			else {
				players[i].setBet(0);
			}
			if (players[i].getHands() == 2) { // Check second hand win loss
				if (plrVal2 > 21) {
					players[i].setBet2(0);
				} else if (plrVal2 == 21 && dealerHand == 21) {
					players[i].setBal(players[i].getBal() + players[i].getBet2());
					players[i].setBet2(0);
				}
				else if (plrVal2 == 21) {
					players[i].setBal(players[i].getBal() + (players[i].getBet2() * 2.5));
					players[i].setBet2(0);
				}
				else if (plrVal2 < 21 && plrVal > dealerHand) {
					players[i].setBal(players[i].getBal() + (players[i].getBet2() * 2.5));
					players[i].setBet2(0);
				}
				else {
					players[i].setBet2(0);
				}
			}
		}
		// Reset Field
		for (int i = 0; i < players.size(); i++) {
			players[i].hand.clear();
			players[i].hand.resize(2);
			if (players[i].getHands() == 2) {
				players[i].setHands(1);
				players[i].hand2.clear();
			}
		}
		// End Game?
		char end;
		smallBox("Would you like to end the match? (y/n)");
		cin.get(end);
		end = toupper(end);
		while (end != 'Y' && end != 'N') {
			cin.ignore(1);
			cin.get(end);
			end = toupper(end);
		}
		if (end == 'Y') {
			playing = false;
			for (int i = 0; i < players.size(); ++i) {
				updateAcc(accPath, players[i]);
			}
		}
	}
	system("pause");
}
