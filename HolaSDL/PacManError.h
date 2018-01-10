#pragma once
#include <stdexcept>
#include <iostream>

using namespace std;

class PacManError : public logic_error {
public:
	PacManError(const char& m) : logic_error("PacMan error " + m) {};
};