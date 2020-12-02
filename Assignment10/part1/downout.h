#ifndef downout
#define downout

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <vector>

using namespace std;

class Down_and_Out_Option
{
public:
    double risk_free_rate, strike_price, barrier_price, R;
    double initial_stock_price, expiration_time, volatility;
    int no_of_divisions;
    int no_of_trials;
    void Init(char *argv[]);
    void Print();

private:
    double max(double a, double b);
    double N(const double &z);
    double get_uniform();
    double option_price_put_black_scholes(const double &S,     // spot price
                                          const double &K,     // Strike (exercise) price,
                                          const double &r,     // interest rate
                                          const double &sigma, // volatility
                                          const double &time);
    double option_price_call_black_scholes(const double &S,     // spot (underlying) price
                                           const double &K,     // strike (exercise) price,
                                           const double &r,     // interest rate
                                           const double &sigma, // volatility
                                           const double &time); // time to maturity
    double closed_form_down_and_out_european_call_option();
    double closed_form_down_and_in_european_put_option();
    double closed_form_down_and_out_european_put_option();
    double adjusted_prob(double stock_price);
    vector<double> Monte_Carlo();
};
#endif