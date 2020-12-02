//Assignment 6
/*
    I shuffle a deck of cards (with an equal number of red and black cards) and start dealing them face up.
    After any card you can say “stop”, at which point I pay you $1 for every red card dealt and you pay me $1 for every black card dealt.
    What is your optimal strategy, and how much would you pay to play this game?
*/
// Written for IE523: Financial Computation by A.Tursynbekova
//
#include <iostream>
#include <string>
#include "nap.h"

int main(int argc, char *argv[])
{
    no_arbitrage_price game;

    if (argc == 1)
    {
        cout << "Input deck size is missing" << endl;
    }
    int deck = stoi((string)argv[1]) / 2;
    cout << "Total Number of Cards = " << argv[1] << endl;
    cout << "Value of the game = " << game.value(deck, deck) << endl;
}
