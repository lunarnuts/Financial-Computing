// Soduku Solver using Brute-Force Search implemted using
// recursion.
// Written for IE523: Financial Computation by Prof. Sreenivas
// and GE523: Discrete Event Systems
//
#include <iostream>
#include "sudoku.h"

using namespace std;

int main(int argc, char *const argv[])
{
    Sudoku x;
    x.read_puzzle(argc, argv);
    x.solCount = 0;
    cout << "Initial:"
         << endl;
    x.print_puzzle();
    x.SolveReverse(0, 0);
    bool ans = x.Solve(0, 0);
    return 0;
}