#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include "european_option.h"
using namespace std;

double up_factor, uptick_prob, risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;

int main(int argc, char *argv[])
{
    European_option option;
    option.Init(argv);
    option.Print();
}
