#pragma once
#include <vector>
#include <iostream>
#include <Logger.hpp>

template<typename T>
using vector_2d = std::vector<std::vector<T>>;

void assert(bool assertion);
void assert(bool assertion, std::string msg);
bool is_number(std::string str);
extern Logger logger;