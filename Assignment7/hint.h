/*
 *  alice_and_bob.h
 *  Loosing as little as possible
 *
 *  Created by Ramavarapu Sreenivas on 9/2/12.
 *  Copyright 2012 University of Illinois at Urbana-Champaign. All rights reserved.
 *
 */
#ifndef ALICE_AND_BOB
#define ALICE_AND_BOB

#include <cmath>
#include <map>
#include <random>

using namespace std;

class I_have_nothing_apropos_for_this_class
{
private:
	map<pair<int, int>, double> valueMap;
	double alice_probability, bob_probability;

	// private member function: uniform RV generator
	double get_uniform()
	{
		//srand((unsigned)time(NULL));
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
		//well technically it is not a real uniform distribution
		// just the gives a random value from MAX_RAND values evenly distributed between 0 and 1
		// this is primarily done as a approximation and for speediness of algo
		//here is more real uniform RV generator but it takes 30mins or more to process with 10^6 samples
		// std::random_device rand_dev;
		// std::mt19937 generator(rand_dev());
		// std::uniform_real_distribution<double> distribution(0.0, 1.0);
		// return distribution(generator);
		// write the appropriate code here
	}

	// private member function: nCi (i.e. n-take-i)
	int take(int n, int i)
	{
		// write a **RECURSIVE** implementation of n-take-i.
		// If you made it non-recurisive (i.e. n!/((n-i)!i!)) -- it
		// will take too long for large sizes
		if (i == 0)
		{
			return 1;
		}
		map<pair<int, int>, double>::iterator it = valueMap.find({n, i});
		if (it != valueMap.end())
			return valueMap[{n, i}]; //memoiziation
		int result = (take(n, i - 1) * (n - i + 1) / i);
		valueMap[{n, i}] = result;
		return result;
	}

	// this routine implements the probability that Alice has more
	// heads than Bob after n-many coin tosses
	double theoretical_value(double q, double p, int n)
	{
		// implement equation 1.1 of Addona-Wagon-Wilf paper
		double sum_qs = 0, alice_chance_of_win = 0;
		int r, s;
		for (r = 0; r <= n; r++) //loop to sum alice's chance of winning
		{
			sum_qs = 0;
			for (s = r + 1; s <= n; s++) //second loop is for summing probability for all s<r
			{
				sum_qs += (double)take(n, s) * pow(q, s) * pow(1 - q, n - s);
				//second summing operator in eq 1.1
			}
			sum_qs *= (double)take(n, r) * pow(p, r) * pow(1 - p, n - r); //first summing operator in eq 1.1
			alice_chance_of_win += sum_qs;
		}
		return alice_chance_of_win;
	}

public:
	// public function:
	void set_probability(double alice_p, double bob_p)
	{
		alice_probability = alice_p;
		bob_probability = bob_p;
	}

	// probability of Alice winning the game.
	double simulated_value(int number_of_coin_tosses_in_each_game, int no_of_trials)
	{
		int no_of_wins_for_alice = 0;
		for (int i = 0; i < no_of_trials; i++)
		{
			int number_of_heads_for_alice = 0;
			int number_of_heads_for_bob = 0;
			for (int j = 0; j < number_of_coin_tosses_in_each_game; j++)
			{
				if (get_uniform() < alice_probability)
					number_of_heads_for_alice++;
				if (get_uniform() < bob_probability)
					number_of_heads_for_bob++;
			}
			if (number_of_heads_for_alice > number_of_heads_for_bob)
				no_of_wins_for_alice++;
		}
		//cout << (((double)no_of_wins_for_alice) / ((double)no_of_trials)) << endl;
		return (((double)no_of_wins_for_alice) / ((double)no_of_trials));
	}

	int search_result()
	{
		// implememt a discrete-search procedure for the optimal n-value.
		// start with n = 1 and find the discrete-value of n that has
		// the largest probability for Alice winning.  Why would this work?
		// See Theorem 2.2 of the paper for the reason!
		// colde according to the theorem 2.2 to find the optimal number.
		float f_of_n = 0, f_of_n_minus_one = 0, f_of_n_plus_one = 0;
		int n = 0;
		do
		{
			n++;
			f_of_n_minus_one = theoretical_value(alice_probability, bob_probability, n - 1);
			f_of_n = theoretical_value(alice_probability, bob_probability, n);
			f_of_n_plus_one = theoretical_value(alice_probability, bob_probability, n + 1);

		} while (!(f_of_n >= f_of_n_minus_one && f_of_n >= f_of_n_plus_one));
		//according to Theorem 2.2 if F(n) >= F(n-1) and F(n) >=F(n+1), then F(n) is the maximum probability of wins
		//and n is number of tosses when that is achived
		return n;
	}
	int plotGraph(int no_of_trials)
	{
		ofstream myFile;
		myFile.open("plotData.dat");
		double t, s;
		myFile << "#Tosses Theory Simulated" << endl;
		for (int n = 1; n <= 30; n++)
		{
			t = theoretical_value(alice_probability, bob_probability, n);
			s = simulated_value(n, no_of_trials);
			myFile << n << " " << t << " " << s << endl;
		}
		myFile.close();
		return 0;
	}
};
#endif
