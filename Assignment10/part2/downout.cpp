#include "downout.h"

unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator;

double Down_and_Out_Option::max(double a, double b)
{
    return (b < a) ? a : b;
}

double Down_and_Out_Option::N(const double &z)
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

// double Down_and_Out_Option::get_uniform()
// {
//     // http://www.cplusplus.com/reference/random/exponential_distribution/
//     std::uniform_real_distribution<double> distribution(0.0, 1.0);
//     double number = distribution(generator);
//     return (number);
// }

double Down_and_Out_Option::get_uniform()
{
    double result = ((double)rand() / ((double)RAND_MAX + 1));
    if (result == 1.0)
    {
        return get_uniform();
        //this methods is really fast, it takes only 6s seconds with 10^6 samples
        //as simply dividing random number by Rand_max may produce value 1.0
        // which is against the function itself, if random number equals 1.0 then compute again recursively
        //this method is referenced from paper:
        // 			Frédéric Goualard. Generating Random Floating-Point Numbers by Dividing Integers: a Case Study.
        // 			Proceedings of ICCS 2020, Jun 2020, Amsterdam, Netherlands. ffhal-02427338f
        //			https://hal.archives-ouvertes.fr/hal-02427338/document
    }
    return result;
}

double Down_and_Out_Option::adjusted_prob(double stock_price) //lesson7 part 1.1
{
    double probability = 1.0;
    double mean, variance;
    for (int j = 1; j <= no_of_divisions; j++)
    {
        mean = initial_stock_price + (((double)j) / ((double)no_of_divisions) * (stock_price - initial_stock_price));

        variance = (((double)j) / ((double)no_of_divisions)) * expiration_time * (1.0 - ((double)j) / ((double)no_of_divisions));

        probability *= (1.0 - N((barrier_price - mean) / sqrt(variance)));
    }
    return probability;
}

vector<double> Down_and_Out_Option::Monte_Carlo() //montecarlo simulation with 4 path for price of 1 path
{
    // chop expiration time into no_of_divisions many segments
    // figure out the motion within each segment
    double delta_t = expiration_time / ((double)no_of_divisions);
    double delta_R = (risk_free_rate - 0.5 * pow(volatility, 2)) * delta_t;
    double delta_SD = volatility * sqrt(delta_t);
    // by sharing random variables we create 4 paths
    double current_stock_price1 = initial_stock_price;
    double current_stock_price2 = initial_stock_price;
    double current_stock_price3 = initial_stock_price;
    double current_stock_price4 = initial_stock_price;

    double K1 = 1, K2 = 1, K3 = 1, K4 = 1; //coeficients to knock out prices, if stock price breaches barrier price
    for (int i = 0; i < no_of_divisions; i++)
    {
        double x = get_uniform();
        double y = get_uniform();
        double a = sqrt(-2.0 * log(x)) * cos(6.283185307999998 * y);
        double b = sqrt(-2.0 * log(x)) * sin(6.283185307999998 * y);

        current_stock_price1 = current_stock_price1 * exp(delta_R + delta_SD * a);
        current_stock_price2 = current_stock_price2 * exp(delta_R - delta_SD * a);
        current_stock_price3 = current_stock_price3 * exp(delta_R + delta_SD * b);
        current_stock_price4 = current_stock_price4 * exp(delta_R - delta_SD * b);

        if (current_stock_price1 <= barrier_price)
            K1 = 0;
        if (current_stock_price2 <= barrier_price)
            K2 = 0;
        if (current_stock_price3 <= barrier_price)
            K3 = 0;
        if (current_stock_price4 <= barrier_price)
            K4 = 0;
    }
    double call_option_price = 0.0;
    double put_option_price = 0.0;
    double adjusted_call_option_price = 0.0;
    double adjusted_put_option_price = 0.0;

    //summing and finding averages for prices by dividing by 4
    call_option_price += K1 * max(0.0, current_stock_price1 - strike_price);
    call_option_price += K2 * max(0.0, current_stock_price2 - strike_price);
    call_option_price += K3 * max(0.0, current_stock_price3 - strike_price);
    call_option_price += K4 * max(0.0, current_stock_price4 - strike_price);

    put_option_price += K1 * max(0.0, strike_price - current_stock_price1);
    put_option_price += K2 * max(0.0, strike_price - current_stock_price2);
    put_option_price += K3 * max(0.0, strike_price - current_stock_price3);
    put_option_price += K4 * max(0.0, strike_price - current_stock_price4);

    adjusted_call_option_price += max(0.0, current_stock_price1 - strike_price) * adjusted_prob(current_stock_price1);
    adjusted_call_option_price += max(0.0, current_stock_price2 - strike_price) * adjusted_prob(current_stock_price2);
    adjusted_call_option_price += max(0.0, current_stock_price3 - strike_price) * adjusted_prob(current_stock_price3);
    adjusted_call_option_price += max(0.0, current_stock_price4 - strike_price) * adjusted_prob(current_stock_price4);

    adjusted_put_option_price += max(0.0, strike_price - current_stock_price1) * adjusted_prob(current_stock_price1);
    adjusted_put_option_price += max(0.0, strike_price - current_stock_price2) * adjusted_prob(current_stock_price2);
    adjusted_put_option_price += max(0.0, strike_price - current_stock_price3) * adjusted_prob(current_stock_price3);
    adjusted_put_option_price += max(0.0, strike_price - current_stock_price4) * adjusted_prob(current_stock_price4);

    call_option_price = exp(-risk_free_rate * expiration_time) * call_option_price / 4.0;
    put_option_price = exp(-risk_free_rate * expiration_time) * put_option_price / 4.0;
    adjusted_call_option_price = exp(-risk_free_rate * expiration_time) * adjusted_call_option_price / 4.0;
    adjusted_put_option_price = exp(-risk_free_rate * expiration_time) * adjusted_put_option_price / 4.0;

    vector<double> montecarlo(4);
    montecarlo[0] = call_option_price;
    montecarlo[1] = put_option_price;
    montecarlo[2] = adjusted_call_option_price;
    montecarlo[3] = adjusted_put_option_price;
    return montecarlo;
}

void Down_and_Out_Option::Init(char *argv[])
{
    sscanf(argv[1], "%lf", &expiration_time);
    sscanf(argv[2], "%lf", &risk_free_rate);
    sscanf(argv[3], "%lf", &volatility);
    sscanf(argv[4], "%lf", &initial_stock_price);
    sscanf(argv[5], "%lf", &strike_price);
    sscanf(argv[6], "%d", &no_of_trials);
    sscanf(argv[7], "%d", &no_of_divisions);
    sscanf(argv[8], "%lf", &barrier_price);
}
void Down_and_Out_Option::Print()
{
    cout << "European Down-and-Out Discrete Barrier Options Pricing via Monte Carlo Simulation" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "Barrier Price = " << barrier_price << endl;
    cout << "Number of Trials = " << no_of_trials << endl;
    cout << "Number of Discrete Barriers = " << no_of_divisions << endl;
    cout << "--------------------------------------" << endl;

    double call_option_price = 0.0;
    double put_option_price = 0.0;
    double adjusted_call_option_price = 0.0;
    double adjusted_put_option_price = 0.0;

    for (int i = 0; i < no_of_trials; i++)
    {
        vector<double> prices = Monte_Carlo();
        call_option_price += prices[0];
        put_option_price += prices[1];
        adjusted_call_option_price += prices[2];
        adjusted_put_option_price += prices[3];
    }
    double average_call_option_price = call_option_price / (double)no_of_trials;
    double average_put_option_price = put_option_price / (double)no_of_trials;
    double average_adjusted_call_option_price = adjusted_call_option_price / (double)no_of_trials;
    double average_adjusted_put_option_price = adjusted_put_option_price / (double)no_of_trials;

    cout << "The average Call Price by explicit simulation of price paths             = " << average_call_option_price << endl;
    cout << "The average  Call Price by Brownian Bridge correction on the final price = " << average_adjusted_call_option_price << endl;
    cout << "The average  Put Price by explicit simulation of price paths             = " << average_put_option_price << endl;
    cout << "The average  Put Price by Brownian Bridge correction on the final price  = " << average_adjusted_put_option_price << endl;
    cout << "--------------------------------------" << endl;
}