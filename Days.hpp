#ifndef DAYS_HPP
#define DAYS_HPP

#include <sstream>

#include "Days.cpp"

std::stringstream get_stringstream(char* filename, char delimiter);

int intcode_computer(std::vector<int> nums, int id);

int mode(vector<int>& vec, int mode, int pos);

int day1(char* filename);
int day1_02(char* filename);

int day2(char* filename);
int day2_02(char* filename);

int day3(char* filename);

int day4();

int day5(char* filename, int id);

int day6(char* filename);

int day7(char* filename);

#endif //DAYS_HPP
