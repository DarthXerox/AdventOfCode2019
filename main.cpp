#include <iostream>
#include <cstdlib>

#include "Days.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Pass exactly two parameters to the program, number of the day and the input file name"<<std::endl;
        return 1;
    }
    char* infile = argv[2];

    switch (atoi(argv[1])) {
        case 1:
            day1(infile);
            day1_02(infile);
            break;
        case 2:
            day2(infile);
            day2_02(infile);
            break;
        case 3:
            day3(infile);
            break;
        case 4:
             day4();
             break;
        case 5:
            day5(infile, 1);
            day5(infile, 5);
            break;
        case 6:
            day6(infile);
            break;
        case 7:
            day7(infile);
            break;
        case 8:
            day8(infile);
            break;
        case 9:
            day9(infile);
            break;
        default:
            std::cout << "Wrong day number" << std::endl;
            return 1;
        }

    return 0;
}
