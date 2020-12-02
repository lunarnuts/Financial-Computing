// k_hanoi.h
// Written by Prof. Sreenivas for IE523

#ifndef K_HANOI_H
#define K_HANOI_H

#include <cmath>
#include <deque>
#include <iostream>
using namespace std;
using std::deque;

class Towers
{
	// private data member: #steps
	int number_of_steps;

	// private data member: #pegs
	int number_of_pegs;

	// private data member: #disks on 1st Peg
	int number_of_disks;

	// private data member: k-Deques-of-Deques
	// towers[0] is deque for the 1st Peg, towers[1] is the deque
	// for the 2nd Peg,..., towers[k-1] is the deque for the kth Peg
	deque<deque<int>> towers;

	// private member function:  returns 'false' if a larger disk
	// is moved on top of a smaller disk during a move
	bool is_everything_legal(int source, int dest)
	{
		int peg1 = 0, peg2 = 0;
		peg1 = towers[source].front();
		if (towers[dest].empty() == true)
			return true;
		peg2 = towers[dest].back();
		return (peg1 < peg2) ? true : false;
	}

	// private member function: initializes the pegs and #disks on Peg 1
	void initialize(int m, int n)
	{
		// write the code for this part
		number_of_pegs = m;
		number_of_disks = n;
		number_of_steps = 0;
		deque<int> tower;
		for (int i = 2; i < m; i++)
		{
			towers.push_back(tower);
		}
		for (int i = 1; i <= n; i++)
		{
			tower.push_back(i);
		}
		towers.push_front(tower);
	}

	// private member function:  picks the #disks to be moved based on
	// the #free pegs that are available at a given instant
	int pick_the_right_number_to_move(int number_of_disks, int number_of_free_pegs)
	{
		if (1 == number_of_free_pegs)
			return (number_of_disks - 1);
		else
			return (number_of_disks / 2);
	}

	// private member function: primitive/basic move of the top disk
	// from the source peg to the destination peg -- and legality-check
	void move_top_disk(int source, int dest)
	{
		if (is_everything_legal(source, dest))
		{
			int temp = towers[source].front();
			towers[dest].push_front(towers[source].front());
			towers[source].pop_front();
			cout << "Move disk " << temp << " from Peg " << source + 1 << " to Peg " << dest + 1 << " (Legal)" << endl;
		}
		// write the code for this part
	}

	// private member function: recursive solution to the 3 Peg Tower of Hanoi
	void Move(int number_of_disks, int source_peg_number, int destination_peg_number, deque<int> free_peg_numbers)
	{
		int temp;

		if (free_peg_numbers.size() != 0)
		{
			if (number_of_disks > 1)
			{
				int m = pick_the_right_number_to_move(number_of_disks, free_peg_numbers.size());
				temp = free_peg_numbers.back();
				//chose a peg from deque of free pegs as buffer to transfer disks
				free_peg_numbers.pop_back();
				free_peg_numbers.push_back(destination_peg_number);
				// 1)Let p be some number that is less than n. Move p disks from the top of peg
				// 1 to the i-th peg, where i ∈ {2, 3, . . . , K−1} using move(p, 1, i, {2, 3, . . . , K}− {i}).
				Move(m, source_peg_number, temp, free_peg_numbers);

				//2)Move the n−p disks from peg1 to peg K using move(n−p,1,K,{2,3,...,K− 1} − {i}).
				//That is, this move does not involve/touch the i-th peg.
				free_peg_numbers.pop_back();
				Move(number_of_disks - m, source_peg_number, destination_peg_number, free_peg_numbers);

				//3)Move the p disks from peg i to peg K using move(p,i,K,{1,2,...,K − 1} − {i}).
				free_peg_numbers.push_back(source_peg_number);
				Move(m, temp, destination_peg_number, free_peg_numbers);
				// write the code for this part
			}
			else
			{
				// number_of_disks == 1
				number_of_steps++;
				move_top_disk(source_peg_number, destination_peg_number);
			}
		}
		else
		{
			// there are no free pegs
			number_of_steps++;
			move_top_disk(source_peg_number, destination_peg_number);
		}
	}
	void print_peg_state(int m)
	{
		// write the code for this part
		cout << "State of Peg " << m + 1 << " (Top to Bottom): ";
		deque<int> tower = towers[m];
		for (int i = 0; i < tower.size(); ++i)
		{
			cout << tower[i] << " ";
		}
		cout << endl;
	}

public:
	// Solves the K Peg Tower of Hanoi
	void solve(int number_of_pegs, int number_of_disks)
	{
		initialize(number_of_pegs, number_of_disks);
		cout << "_________________________" << endl;
		print_peg_state(0);
		cout << "Number of Steps = " << number_of_steps << endl;
		cout << "_________________________" << endl;

		deque<int> free_peg_numbers;
		// source peg # is 0; destination peg # is no_of_pegs-1;
		// free pegs {1,.., no_of_pegs-2}
		for (int i = 1; i <= number_of_pegs - 2; i++)
			free_peg_numbers.push_back(i);
		Move(number_of_disks, 0, number_of_pegs - 1, free_peg_numbers);
		cout << "_________________________" << endl;
		print_peg_state(number_of_pegs - 1);
		cout << "Number of Steps = " << number_of_steps << endl;
		cout << "_________________________" << endl;
	}
};
#endif