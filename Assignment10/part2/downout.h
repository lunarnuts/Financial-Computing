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
    double adjusted_prob(double stock_price);
    vector<double> Monte_Carlo();
};
#endif