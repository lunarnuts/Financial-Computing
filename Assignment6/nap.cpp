#include "nap.h"

double no_arbitrage_price::value(int redCards, int blackCards)
{
    double total;
    double result;
    //base condition for recursion:
    //if none of blackCards cards are left, then to minimize loses we draw all deck
    //if none of redCards cards are left, then profit is (total-redCards) which is number of blackCards cards
    if (redCards == 0)
        return blackCards;
    if (blackCards == 0)
        return 0;
    //memoization implementation: find given key,value pair in map, if found return item(value) from map
    map<pair<int, int>, double>::iterator it = valueMap.find({redCards, blackCards});
    if (it != valueMap.end())
        return valueMap[{redCards, blackCards}];

    // If value(#redCards cards left, #blackCards cards left) is a function that computes the value of the game to you at any point, then
    // value(#Red cards left, #Black cards left)) = max {(Prob of Red Card drawn) × value(#Red cards left-1, #Black cards left))+ (Prob of Black Card drawn) × value(#Red cards left, #Black cards left - 1)),
    // (#Black cards left - #Red cards left) (1)
    total = redCards + blackCards;
    result = max((redCards / total) * value(redCards - 1, blackCards) + (blackCards / total) * value(redCards, blackCards - 1), blackCards - redCards);

    //save result to valueMap
    valueMap[{redCards, blackCards}] = result;

    return result;
}

double no_arbitrage_price::max(double value1, double value2)
{
    return value1 > value2 ? value1 : value2;
}