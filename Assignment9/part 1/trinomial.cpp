#include "european_option.h"

void European_option::Init(char *argv[])
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
    vect.assign(size, -1.0);
    vector<vector<double>> temp;
    temp.assign(no_of_divisions + 1, vect);
    Call_Option_Price = temp;
    Put_Option_Price = temp;
};

void European_option::Print()
{
    cout << "(Memoized) Recursive Trinomial European Option Pricing" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Up Factor = " << up_factor << endl;
    cout << "Uptick Probability = " << uptick_prob << endl;
    cout << "--------------------------------------" << endl;
    double call_price = European_call_option(0, 0);
    cout << "Trinomial Price of an European Call Option = " << call_price << endl;
    cout << "Call Price according to Black-Scholes = " << option_price_call_black_scholes(initial_stock_price, strike_price, risk_free_rate, volatility, expiration_time) << endl;
    cout << "--------------------------------------" << endl;
    double put_price = European_put_option(0, 0);
    cout << "Trinomial Price of an European Put Option = " << put_price << endl;
    cout << "Put Price according to Black-Scholes = " << option_price_put_black_scholes(initial_stock_price, strike_price, risk_free_rate, volatility, expiration_time) << endl;
    cout << "--------------------------------------" << endl;
    cout << "Verifying Put-Call Parity: S+P-C = Kexp(-r*T)" << endl;
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

double European_option::European_call_option(int k, int i)
{
    double uptick_price, notick_price, downtick_price, result;
    if (Call_Option_Price[k][i + no_of_divisions] > -1.0)
        return Call_Option_Price[k][i + no_of_divisions];

    if (k == no_of_divisions)
    {
        result = max(0.0, initial_stock_price * pow(up_factor, (double)i) - strike_price);
        Call_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
    else
    {
        uptick_price = uptick_prob * European_call_option(k + 1, i + 1);
        downtick_price = downtick_prob * European_call_option(k + 1, i - 1);
        notick_price = notick_prob * European_call_option(k + 1, i);
        //trinomial tree for uptick, downtick and notick prices for call option
        result = (uptick_price + downtick_price + notick_price) / R;
        Call_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
}

double European_option::European_put_option(int k, int i)
{
    double uptick_price, notick_price, downtick_price, result;
    if (Put_Option_Price[k][i + no_of_divisions] > -1.0)
        return Put_Option_Price[k][i + no_of_divisions];

    if (k == no_of_divisions)
    {
        result = max(0.0, strike_price - initial_stock_price * pow(up_factor, (double)i));
        Put_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
    else
    {
        uptick_price = uptick_prob * European_put_option(k + 1, i + 1);
        downtick_price = downtick_prob * European_put_option(k + 1, i - 1);
        notick_price = notick_prob * European_put_option(k + 1, i);
        //trinomial tree for uptick, downtick and notick prices for call option
        result = (uptick_price + downtick_price + notick_price) / R;
        Put_Option_Price[k][i + no_of_divisions] = result;
        return result;
    }
};
double European_option::max(double a, double b)
{
    return (b < a) ? a : b;
}

double European_option::option_price_put_black_scholes(const double &S,     // spot price
                                                       const double &K,     // Strike (exercise) price,
                                                       const double &r,     // interest rate
                                                       const double &sigma, // volatility
                                                       const double &time)
{
    double time_sqrt = sqrt(time);
    double d1 = (log(S / K) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
    double d2 = d1 - (sigma * time_sqrt);
    return K * exp(-r * time) * N(-d2) - S * N(-d1);
};

double European_option::option_price_call_black_scholes(const double &S,     // spot (underlying) price
                                                        const double &K,     // strike (exercise) price,
                                                        const double &r,     // interest rate
                                                        const double &sigma, // volatility
                                                        const double &time)
{ // time to maturity
    double time_sqrt = sqrt(time);
    double d1 = (log(S / K) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
    double d2 = d1 - (sigma * time_sqrt);
    return S * N(d1) - K * exp(-r * time) * N(d2);
};

double European_option::N(const double &z)
{
    if (z > 6.0)
    {
        return 1.0;
    }; // this guards against overflow
    if (z < -6.0)
    {
        return 0.0;
    };
    double b1 = 0.31938153;
    double b2 = -0.356563782;
    double b3 = 1.781477937;
    double b4 = -1.821255978;
    double b5 = 1.330274429;
    double p = 0.2316419;
    double c2 = 0.3989423;
    double a = fabs(z);
    double t = 1.0 / (1.0 + a * p);
    double b = c2 * exp((-z) * (z / 2.0));
    double n = ((((b5 * t + b4) * t + b3) * t + b2) * t + b1) * t;
    n = 1.0 - b * n;
    if (z < 0.0)
        n = 1.0 - n;
    return n;
};