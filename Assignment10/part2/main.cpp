#include "downout.h"

int main(int argc, char *argv[])
{
    Down_and_Out_Option option;
    option.Init(argv);
    option.Print();
}