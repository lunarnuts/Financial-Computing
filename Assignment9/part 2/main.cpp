#include "american_option.h"
int main(int argc, char *argv[])
{
    American_option option;
    option.Init(argv);
    option.Print();
}