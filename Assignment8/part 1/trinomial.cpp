#include "american_option.h"

void American_option::Init(char *argv[])
{
    sscanf(argv[1], "%lf", &this->expiration_time);
    sscanf(argv[2], "%d", &this->no_of_divisions);
    sscanf(argv[3], "%lf", &this->risk_free_rate);
    sscanf(argv[4], "%lf", &this->volatility);
    sscanf(argv[5], "%lf", &this->initial_stock_price);
    sscanf(argv[6], "%lf", &this->strike_price);
    up_factor = exp(volatility * sqrt(2 * expiration_time / ((float)no_of_divisions)));
    down_factor = (float)1.0 / up_factor;
    R = exp(risk_free_rate * expiration_time / ((float)no_of_divisions));
    uptick_prob = pow((sqrt(R) - sqrt(down_factor)) / (sqrt(up_factor) - sqrt(down_factor)), 2);
    downtick_prob = pow((sqrt(up_factor) - sqrt(R)) / (sqrt(up_factor) - sqrt(down_factor)), 2);
    notick_prob = (float)(1.0 - (uptick_prob + downtick_prob));
};

void American_option::Print()
{
    cout << "Recursive Trinomial American Option Pricing" << endl;
    cout << "Expiration Time (Years) = " << this->expiration_time << endl;
    cout << "Number of Divisions = " << this->no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << this->risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << this->volatility * 100 << endl;
    cout << "Initial Stock Price = " << this->initial_stock_price << endl;
    cout << "Strike Price = " << this->strike_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "R = " << this->R << endl;
    cout << "Up Factor = " << this->up_factor << endl;
    cout << "Down Factor = " << this->down_factor << endl;
    cout << "Uptick Probability = " << this->uptick_prob << endl;
    cout << "Downtick Probability = " << this->downtick_prob << endl;
    cout << "Notick Probability = " << this->notick_prob << endl;
    cout << "--------------------------------------" << endl;
    double call_price = american_call_option(0, 0, this->initial_stock_price);
    cout << "Trinomial Price of an American Call Option = " << call_price << endl;
    double put_price = american_put_option(0, 0, this->initial_stock_price);
    cout << "Trinomial Price of an American Put Option = " << put_price << endl;
    cout << "--------------------------------------" << endl;
};

double American_option::american_call_option(int k, int i, double current_stock_price)
{
    double uptick_price, notick_price, downtick_price;
    if (k == no_of_divisions)
        return max(0.0, (current_stock_price - strike_price));
    else
    {
        uptick_price = uptick_prob * american_call_option(k + 1, i + 1, current_stock_price * up_factor);
        downtick_price = downtick_prob * american_call_option(k + 1, i - 1, current_stock_price / up_factor);
        notick_price = notick_prob * american_call_option(k + 1, i, current_stock_price);
        //trinomial tree for uptick, downtick and notick prices for call option
        return max((current_stock_price - strike_price), (uptick_price + downtick_price + notick_price) / R);
    }
}

double American_option::american_put_option(int k, int i, double current_stock_price)
{
    double uptick_price, notick_price, downtick_price;
    if (k == no_of_divisions)
        return max(0.0, (strike_price - current_stock_price));
    else
    {
        uptick_price = uptick_prob * american_put_option(k + 1, i + 1, current_stock_price * up_factor);
        downtick_price = downtick_prob * american_put_option(k + 1, i - 1, current_stock_price / up_factor);
        notick_price = notick_prob * american_put_option(k + 1, i, current_stock_price);
        //trinomial tree for uptick, downtick and notick prices for put option
        return max((strike_price - current_stock_price), (uptick_price + downtick_price + notick_price) / R);
    }
};