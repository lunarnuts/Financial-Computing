//
// Created by kenny on 19.09.20.
//
#include "sudoku.h"
#include <iostream>
#include <fstream>

using namespace std;

void Sudoku::print_puzzle()
{
    if (solCount == 0)
    {
        std::cout << std::endl
                  << "Board Position" << std::endl;
    }
    else
    {

        std::cout << std::endl
                  << "Solution #" << solCount << endl
                  << "Board Position" << std::endl;
    }
    solCount++;
    for (const auto &row : puzzle)
    {
        for (const int &num : row)
        {
            // check if we have a legitimate integer between 1 and 9
            if ((num >= 1) && (num <= 9))
            {
                // printing initial value of the puzzle with some formatting
                std::cout << num << " ";
            }
            else
            {
                // printing initial value of the puzzle with some formatting
                std::cout << "X ";
            }
        }
        std::cout << std::endl;
    }
}

bool Sudoku::row_valid(int row) const
{
    unordered_set<int> values;
    for (const int &num : puzzle[row])
    {
        if (!check_set(values, num))
            return false;
    }
    return true;
}

bool Sudoku::col_valid(int col) const
{
    unordered_set<int> values;
    for (const auto &row : puzzle)
    {
        if (!check_set(values, row[col]))
            return false;
    }
    return true;
}

bool Sudoku::block_valid(int row, int col) const
{
    unordered_set<int> values;
    int block_row = row / 3;
    int block_col = col / 3;
    for (int i = 3 * block_row; i < 3 * (block_row + 1); ++i)
    {
        for (int j = 3 * block_col; j < 3 * (block_col + 1); ++j)
            if (!check_set(values, puzzle[i][j]))
                return false;
    }
    return true;
}

void Sudoku::read_puzzle(int argc, char *const *argv)
{
    if (argc == 2)
    {
        fstream myfile(argv[1], ios_base::in);
        int k, j = 0;
        while (myfile >> k)
        {
            puzzle[j / 9][j % 9] = k;
            puzzleReverse[j / 9][j % 9] = k;
            j++;
        }
    }
    else
    {
        throw invalid_argument("Wrong run format, Help: ./main.cpp <input_file>");
    }
}

bool Sudoku::Solve(int row, int col)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (i < row || (j < col && i == row))
                continue;
            if (puzzle[i][j] == 0)
            {
                for (int k = 1; k < 10; ++k)
                {
                    puzzle[i][j] = k;
                    if (row_valid(i) && col_valid(j) && block_valid(i, j) && Solve(i, j))
                    {
                        return true;
                    }
                    puzzle[i][j] = 0;
                }
                return false;
            }
        }
    }
    print_puzzle();
    if (compareAnswers() == false)
    {
        return false;
    }
    return true;
}
bool Sudoku::compareAnswers()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (puzzle[i][j] != puzzleReverse[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::SolveReverse(int row, int col)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (i < row || (j < col && i == row))
                continue;
            if (puzzleReverse[i][j] == 0)
            {
                for (int k = 9; k > 0; k--)
                {
                    puzzleReverse[i][j] = k;
                    if (row_validR(i) && col_validR(j) && block_validR(i, j) && SolveReverse(i, j))
                    {
                        return true;
                    }
                    puzzleReverse[i][j] = 0;
                }
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::check_set(unordered_set<int> &values, const int &num) const
{
    if (values.count(num))
        return false;

    if (num != 0)
        values.insert(num);

    return true;
}

bool Sudoku::row_validR(int row) const
{
    unordered_set<int> values;
    for (const int &num : puzzleReverse[row])
    {
        if (!check_set(values, num))
            return false;
    }
    return true;
}

bool Sudoku::col_validR(int col) const
{
    unordered_set<int> values;
    for (const auto &row : puzzleReverse)
    {
        if (!check_set(values, row[col]))
            return false;
    }
    return true;
}

bool Sudoku::block_validR(int row, int col) const
{
    unordered_set<int> values;
    int block_row = row / 3;
    int block_col = col / 3;
    for (int i = 3 * block_row; i < 3 * (block_row + 1); ++i)
    {
        for (int j = 3 * block_col; j < 3 * (block_col + 1); ++j)
            if (!check_set(values, puzzleReverse[i][j]))
                return false;
    }
    return true;
}