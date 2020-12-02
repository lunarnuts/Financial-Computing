#ifndef american_option
#define american_option
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;
class European_option
{
public:
    double expiration_time, risk_free_rate, volatility, initial_stock_price, strike_price;
    int no_of_divisions;
    void Init(char *argv[]);
    void Print();

private:
    double up_factor, down_factor, uptick_prob, downtick_prob, notick_prob, R;
    double max(double a, double b);
    vector<vector<double>> Call_Option_Price;
    vector<vector<double>> Put_Option_Price;
    double N(const double &z);
    double European_call_option(int k, int i);
    double European_put_option(int k, int i);
    double option_price_put_black_scholes(const double &S,     // spot price
                                          const double &K,     // Strike (exercise) price,
                                          const double &r,     // interest rate
                                          const double &sigma, // volatility
                                          const double &time);
    double option_price_call_black_scholes(const double &S,     // spot price
                                           const double &K,     // Strike (exercise) price,
                                           const double &r,     // interest rate
                                           const double &sigma, // volatility
                                           const double &time);
};
#endif