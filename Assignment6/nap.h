#ifndef nap
#define nap

#include <map>
using namespace std;

class no_arbitrage_price
{
private:
    //a map to store calculated value
    map<pair<int, int>, double> valueMap;
    //function to give max value of two items
    double max(double value1, double value2);

public:
    //public member function to find no arbitrage price
    double value(int redCards, int blackCards);
};
#endif