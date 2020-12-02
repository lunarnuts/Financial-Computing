// GE523: Discrete Event Systems
// IE511: Integer Programming
// IE523: Financial Computing
//
// A Sudoku Solver in C++ written by Prof. R.S. Sreenivas
// This is written in a "C" like syntax.  You can think about how to
// put this into an include file etc on your own.
//
// There is an ILP formulation for any 9 x 9 Sudoku Puzzle
// See the handout for the formulation -- there are 729 variables
// with a whole lot of constraints that is driven by the input
// (which is the incomplete board-layout).  You can write this
// (moderate-size) ILP automatically in C++, which is then solved
// using the API for Lpsolve.

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>

#include "lp_lib.h"

using namespace std;

// Global variables for the problem 9*9*9 = 729
// See my handout for further explanations
lprec *lp;
double solution[729];

// This sets the generic ILP for the Sudoku Puzzle
// It does not use any of the pre-determined board-positions,
// that part is done after the input file is read.
void set_sudoku_ilp()
{
    // setting the problem up: 9*9*9 = 729 variables
    lp = make_lp(0, 729);

    // This keeps the message reporting of lp_solve to a minimum
    set_verbose(lp, 3);

    {
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                // constraint that says each (i,j) entry in the 9x9 table
                // should have one number in it
                // creating a row with 1's at the right spot (the first
                // entry has to be a zero; this is idiosynchratic of lpsolve)

                double row[730];
                for (int k = 0; k < 730; k++)
                    row[k] = 0;

                for (int k = 1; k <= 9; k++)
                    row[(81 * (i - 1)) + (9 * (j - 1)) + k] = 1;

                // adding the constraint
                add_constraint(lp, row, EQ, 1);
            }
        }
    }

    {
        // Using the above code as a guide, put the appropriate lines that
        // articulate the constraint that says each number must appear once
        // in each row in the 9x9 table; create a bunch of rows with 1's at
        // the right spot (the first entry has to be a zero; this is
        // idiosynchratic of lpsolve)
        for (int i = 1; i <= 9; i++)
        {
            for (int k = 1; k <= 9; k++)
            {
                // constraint that says each number must appear once
                // in each row in 9x9 table;

                double row[730];
                for (int j = 0; j < 730; j++)
                    row[j] = 0;

                for (int j = 1; j <= 9; j++)
                    row[(81 * (i - 1)) + (9 * (j - 1)) + k] = 1;

                // adding the constraint
                add_constraint(lp, row, EQ, 1);
            }
        }
    }

    {
        // Using the above code as a guide, put the appropriate lines that
        // articulate the constraint that says each number must appear once
        // in each column in the 9x9 table; create a bunch of rows with 1's at
        // the right spot (the first entry has to be a zero; this is
        // idiosynchratic of lpsolve)
        for (int j = 1; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                // constraint that says each number must appear once
                // in each colum in 9x9 table;

                double row[730];
                for (int i = 0; i < 730; i++)
                    row[i] = 0;

                for (int i = 1; i <= 9; i++)
                    row[(81 * (i - 1)) + (9 * (j - 1)) + k] = 1;

                // adding the constraint
                add_constraint(lp, row, EQ, 1);
            }
        }
    }

    // making sure each number(k=1...9) occurs once within each block
    // each box is defined by p,q where p = 1...3, q=1...3, thus i=3*p-2..3*p,j=3*q-2..3*q
    {
        for (int k = 1; k <= 9; k++)
            for (int p = 1; p <= 3; p++)
                for (int q = 1; q <= 3; q++)
                {
                    double row[730];
                    for (int i = 0; i < 730; i++) //initializing matrix with zeroes, first index is set to 0 per Lp solve intricacy
                        row[i] = 0;
                    for (int i = (3 * p - 2); i <= (3 * p); i++)
                        for (int j = (3 * q - 2); j <= (3 * q); j++)
                            row[(81 * (i - 1)) + (9 * (j - 1)) + k] = 1;
                    add_constraint(lp, row, EQ, 1);
                }
    }

    // upper-bound each variable by 1
    for (int i = 1; i <= 729; i++)
    {
        double row[730];
        for (int j = 0; j < 730; j++)
            row[j] = 0;
        row[i] = 1;
        add_constraint(lp, row, LE, 1);
    }

    // it does not matter what the objective function (why?) Answer: because it is a feasibility problem,
    // we only check if we can fit into matrix so that all constrainsts can be satisfied
    // minimizing the sum of all variables.
    {
        double row[730];
        for (int i = 1; i < 730; i++)
            row[i] = 1;
        row[0] = 0;

        set_obj_fn(lp, row);
    }

    // set the variables to be integers
    for (int i = 1; i <= 729; i++)
        set_int(lp, i, TRUE);
}

// This subroutine reads the incomplete board-information from the
// input file and sets the appropriate constraints before the ILP
// is solved.
void read_input_data(char *argv[])
{
    // reading the input filename from commandline
    ifstream input_filename(argv[1]);

    if (input_filename.is_open())
    {

        cout << "Input File Name: " << argv[1] << endl;
        cout << endl
             << "Board Position" << endl;
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                int value_just_read;
                input_filename >> value_just_read;

                // check if we have a legitimate integer between 1 and 9
                if ((value_just_read >= 1) && (value_just_read <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    cout << value_just_read << " ";

                    double row[730];
                    for (int k = 0; k < 730; k++)
                        row[k] = 0;

                    row[(81 * (i - 1)) + (9 * (j - 1)) + value_just_read] = 1;

                    // adding the constraint
                    add_constraint(lp, row, EQ, 1);
                    // add appropriate constraints that bind the value of the
                    // appropriate variables based on the incomplete information
                    // that was read from the input file
                }
                else
                {
                    // printing initial value of the puzzle with some formatting
                    cout << "X ";
                }
            }
            cout << endl;
        }
    }
    else
    {
        cout << "Input file missing" << endl;
        exit(0);
    }
}

// The ILP formulation is solved using the API for Lpsolve
// Pay attention to how the solution is interpretted...
void solve_the_puzzle()
{
    int ret;
    // solve the lp
    ret = solve(lp);
    int count = 0;
    // Check if you had a solution
    // (see online guide for the codes at http://lpsolve.sourceforge.net/5.0/ )
    while (ret == 0)
    {
        count++;
        get_variables(lp, solution);
        cout << endl
             << "Solution #" << count << endl
             << "Board position" << endl;
        {
            for (int i = 1; i <= 9; i++)
            {
                for (int j = 1; j <= 9; j++)
                {
                    for (int k = 1; k <= 9; k++)
                    {
                        if (solution[(81 * (i - 1)) + (9 * (j - 1)) + k - 1] == 1)
                        {
                            cout << k << " ";
                        }
                    }
                }
                cout << endl;
            }
        }
        double row[730];
        int ones = 0;
        for (int a = 1; a < 730; a++)
        {
            solution[a - 1] == 1 ? row[a] = 1, ones++ : row[a] = -1;
        }
        row[0] = 0;
        add_constraint(lp, row, LE, ones - 1);
        ret = solve(lp);
    }

    if (count == 0)
    {
        cout << "Check the input file... looks like there is no solution" << endl;
    }

    delete_lp(lp);
}

int main(int argc, char *argv[])
{
    // formulate the non-input related part of the puzzle
    set_sudoku_ilp();

    // read the incomplete input information, and set appropriate constraints
    read_input_data(argv);

    // solve the puzzle and print the solution
    solve_the_puzzle();

    return (0);
}