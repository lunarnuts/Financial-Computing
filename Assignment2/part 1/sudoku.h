#ifndef sudoku
#define sudoku

#include <unordered_set>

class Sudoku {


public:
    // Public member function that reads the incomplete puzzle
    // we are not doing any checks on the input puzzle -- that is,
    // we are assuming they are indeed valid



    // write code that reads the input puzzle using the
    // guidelines of figure 23 of the bootcamp material
    void read_puzzle(int argc, char *const argv[]);


    // Public member function that prints the puzzle when called
    void print_puzzle() const;

    // Public member function that (recursively) implements the brute-force
    // search for possible solutions to the incomplete Sudoku puzzle


    // this part of the code identifies the row and col number of the
    // first incomplete (i.e. 0) entry in the puzzle.  If the puzzle has
    // no zeros, the variable row will be 9 => the puzzle is done, as
    // each entry is row-, col- and block-valid...

    // use the pseudo code of figure 3 of the description
    bool Solve(int row, int col);

private:
    int puzzle[9][9];

    // Private member function that checks if the named row is valid
    bool row_valid(int row) const;

    // Private member function that checks if the named column is valid
    bool col_valid(int col) const;

    // Private member function that checks if the named 3x3 block is valid
    bool block_valid(int row, int col) const;

    bool check_set(std::unordered_set<int> &values, const int &num) const;

};

#endif