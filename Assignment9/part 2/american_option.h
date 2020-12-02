#ifndef american_option
#define american_option
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
class American_option
{
public:
    double expiration_time, risk_free_rate, volatility, initial_stock_price, strike_price;
    int no_of_divisions;
    void Init(char *argv[]);
    void Print();

private:
    double up_factor, down_factor, uptick_prob, downtick_prob, notick_prob, R;
    vector<vector<double>> Call_Option_Price;
    vector<vector<double>> Put_Option_Price;
    double american_call_option(int k, int i, double current_stock_price);
    double american_put_option(int k, int i, double current_stock_price);
    double max(double a, double b);
};
#endif