//
//  main.cpp
//  Stable Marriage Problem
//
//  Created by Ramavarapu Sreenivas on 8/29/14.
//  Copyright (c) 2014 Ramavarapu Sreenivas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class stable_marriage_instance
{
    // Private
    int no_of_couples;
    vector<vector<int>> Preference_of_men;
    vector<vector<int>> Preference_of_women;
    vector<int> match_for_men;
    vector<int> match_for_women;

    // private member function: checks if anybody is free in boolean "my_array"
    // returns the index of the first-person who is free in "my_array"
    // if no one is free it returns a -1.
    int anybody_free(vector<bool> my_array)
    {

        // fill the necessary code for this function
        for (int i = 0; i < no_of_couples; i++)
        {
            if (my_array.at(i) == true)
            {
                return i;
            }
        }
        return -1;
    }

    // private member function: if index1 is ranked higher than index2
    // in a preference array called "my_array" it returns "true"; otherwise
    // it returns "false"
    bool rank_check(vector<int> my_array, int index1, int index2)
    {
        // fill the necessary code for this function
        for (int i = 0; i < no_of_couples; i++)
        {
            if (my_array[i] == index1)
            {
                return true;
            }
            else if (my_array[i] == index2)
            {
                return false;
            }
        }
        return false;
    }

    // private member function: implements the Gale-Shapley algorithm
    void Gale_Shapley()
    {
        vector<bool> is_man_free;
        vector<bool> is_woman_free;
        vector<vector<bool>> has_this_man_proposed_to_this_woman;

        int man_index, woman_index;

        // initializing everything
        for (int i = 0; i < no_of_couples; i++)
        {
            is_man_free.insert(is_man_free.begin() + i, true);
            is_woman_free.insert(is_woman_free.begin() + i, true);
            vector<bool> vec;
            for (int j = 0; j < no_of_couples; j++)
            {
                vec.insert(vec.begin() + j, false);
            }
            match_for_men.insert(match_for_men.begin() + i, -1);
            match_for_women.insert(match_for_women.begin() + i, -1);
            has_this_man_proposed_to_this_woman.insert(has_this_man_proposed_to_this_woman.begin(), vec);
        }

        // Gale-Shapley Algorithm
        int wi = 0;

        while ((man_index = anybody_free(is_man_free)) >= 0)
        {
            // fill the necessary code here
            int w = Preference_of_men[man_index][wi];
            if (is_woman_free[w])
            {
                is_woman_free[w] = false;
                is_man_free[man_index] = false;
                match_for_men[man_index] = w;
                match_for_women[w] = man_index;
                has_this_man_proposed_to_this_woman[man_index][w] = true;
                wi = 0;
            }
            else
            {
                if (rank_check(Preference_of_women[w], man_index, match_for_women[w]))
                {
                    match_for_men[man_index] = w;
                    is_man_free[match_for_women[w]] = true;
                    match_for_women[w] = man_index;
                    is_woman_free[w] = false;
                    is_man_free[man_index] = false;
                    wi = 0;
                }
                else
                {
                    has_this_man_proposed_to_this_woman[man_index][w] = true;
                    wi++;
                }
            }
        }
    }

    // private member function: reads data
    void read_data(int argc, const char *argv[])
    {
        ifstream myfile(argv[1]);
        string line;
        if (myfile.is_open())
        {
            int input;
            myfile >> no_of_couples;
            cout << "Gale-Shapley Algorithm\nInput File Name: " << argv[1] << endl;
            cout << "Number of couples = " << no_of_couples << endl;
            cout << "Preferences of Men\n__________________" << endl;
            for (int i = 0; i < no_of_couples; i++)
            {
                vector<int> pref_of_man;
                cout << "(" << i << "): ";
                for (int j = 0; j < no_of_couples; j++)
                {
                    myfile >> input;
                    cout << input << " ";
                    pref_of_man.insert(pref_of_man.begin() + j, input);
                }
                cout << endl;
                Preference_of_men.insert(Preference_of_men.begin() + i, pref_of_man);
            }
            cout << "\nPreferences of Women\n__________________" << endl;
            for (int i = 0; i < no_of_couples; i++)
            {
                vector<int> pref_of_woman;
                cout << "(" << i << "): ";
                for (int j = 0; j < no_of_couples; j++)
                {
                    myfile >> input;
                    cout << input << " ";
                    pref_of_woman.insert(pref_of_woman.begin() + j, input);
                }
                cout << endl;
                Preference_of_women.insert(Preference_of_women.begin() + i, pref_of_woman);
            }
            cout << endl;
            myfile.close();
        }
    }

    // private member function: print solution
    void print_soln()
    {
        // write the appropriate code here
        cout << "Matching for Men" << endl;
        for (int i = 0; i < no_of_couples; i++)
        {
            cout << "Man: " << i << " -> Woman: " << match_for_men[i] << endl;
        }
        cout << endl;
        cout << "Matching for Women" << endl;
        for (int i = 0; i < no_of_couples; i++)
        {
            cout << "Woman: " << i << " -> Man: " << match_for_women[i] << endl;
        }
        cout << endl;
    }

public:
    void solve_it(int argc, const char *argv[])
    {
        read_data(argc, argv);

        Gale_Shapley();

        print_soln();
    }
};

int main(int argc, const char *argv[])
{
    stable_marriage_instance x;
    x.solve_it(argc, argv);
}
