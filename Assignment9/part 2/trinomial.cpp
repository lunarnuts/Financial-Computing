#include "american_option.h"

void American_option::Init(char *argv[])
{
    sscanf(argv[1], "%lf", &this->expiration_time);
    sscanf(argv[2], "%d", &this->no_of_divisions);
    sscanf(argv[3], "%lf", &this->risk_free_rate);
    sscanf(argv[4], "%lf", &this->volatility);
    sscanf(argv[5], "%lf", &this->initial_stock_price);
    sscanf(argv[6], "%lf", &this->strike_price);

    up_factor = exp(volatility * sqrt((2 * expiration_time) / ((float)no_of_divisions)));
    down_factor = (float)1.0 / up_factor;
    R = exp(risk_free_rate * expiration_time / ((float)no_of_divisions));

    uptick_prob = pow((sqrt(R) - 1 / sqrt(up_factor)) / (sqrt(up_factor) - 1 / sqrt(up_factor)), 2);
    downtick_prob = pow((sqrt(up_factor) - sqrt(R)) / (sqrt(up_factor) - 1 / sqrt(up_factor)), 2);
    notick_prob = 1 - uptick_prob - downtick_prob;

    int size = 2 * no_of_divisions + 1;
    vector<double> vect;
    vect.assign(size, -1);
    Call_Option_Price.assign(no_of_divisions + 1, vect);
    Put_Option_Price.assign(no_of_divisions + 1, vect);
};

void American_option::Print()
{
    cout << "(Memoized) Recursive Trinomial American Option Pricing" << endl;
    cout << "Expiration Time (Years) = " << this->expiration_time << endl;
    cout << "Number of Divisions = " << this->no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << this->risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << this->volatility * 100 << endl;
    cout << "Initial Stock Price = " << this->initial_stock_price << endl;
    cout << "Strike Price = " << this->strike_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Up Factor = " << this->up_factor << endl;
    cout << "Uptick Probability = " << this->uptick_prob << endl;
    cout << "Notick Probability = " << this->notick_prob << endl;
    cout << "Downtick Probability = " << this->downtick_prob << endl;

    cout << "--------------------------------------" << endl;
    double call_price = american_call_option(0, 0, this->initial_stock_price);
    cout << "Trinomial Price of an American Call Option = " << call_price << endl;
    double put_price = american_put_option(0, 0, this->initial_stock_price);
    cout << "Trinomial Price of an American Put Option = " << put_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Verifying Put-Call Parity: S+P-C = Kexp(-r*T) for American Options" << endl;
    cout << initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;

    if (to_string(initial_stock_price + put_price - call_price) == to_string(strike_price * exp(-risk_free_rate * expiration_time)))
    {
        cout << initial_stock_price + put_price - call_price << " =" << strike_price * exp(-risk_free_rate * expiration_time) << endl;
    }
    else
    {
        cout << initial_stock_price + put_price - call_price << " ?=?" << strike_price * exp(-risk_free_rate * expiration_time) << endl;
        cout << "Looks Like PUT-CALL Parity does NOT hold." << endl;
    }
    cout << "--------------------------------------" << endl;
};

double American_option::american_call_option(int k, int i, double current_stock_price)
{
    double uptick_price, notick_price, downtick_price, result;
    if (Call_Option_Price[k][i + no_of_divisions] > -0.1)
        return Call_Option_Price[k][i + no_of_divisions];

    if (k == no_of_divisions)
    {
        result = max(0.0, (current_stock_price - strike_price));
        Call_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
    else
    {
        uptick_price = uptick_prob * american_call_option(k + 1, i + 1, current_stock_price * up_factor);
        notick_price = notick_prob * american_call_option(k + 1, i, current_stock_price);
        downtick_price = downtick_prob * american_call_option(k + 1, i - 1, current_stock_price / up_factor);

        //trinomial tree for uptick, downtick and notick prices for call option
        result = max((current_stock_price - strike_price), (uptick_price + downtick_price + notick_price) / R);
        Call_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
}

double American_option::american_put_option(int k, int i, double current_stock_price)
{
    double uptick_price, notick_price, downtick_price, result;
    if (Put_Option_Price[k][i + no_of_divisions] > -0.1)
        return Put_Option_Price[k][i + no_of_divisions];

    if (k == no_of_divisions)
    {
        result = max(0.0, (strike_price - current_stock_price));
        Put_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
    else
    {
        uptick_price = uptick_prob * american_put_option(k + 1, i + 1, current_stock_price * up_factor);
        downtick_price = downtick_prob * american_put_option(k + 1, i - 1, current_stock_price / up_factor);
        notick_price = notick_prob * american_put_option(k + 1, i, current_stock_price);
        //trinomial tree for uptick, downtick and notick prices for call option
        result = max((strike_price - current_stock_price), (uptick_price + downtick_price + notick_price) / R);
        Put_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
};
double American_option::max(double a, double b)
{
    return (b < a) ? a : b;
}