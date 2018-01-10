#pragma once
#include "PacManError.h"
#include <stdexcept>
#include <iostream>

using namespace std;

class FileNotFoundError : public PacManError {
public:
	FileNotFoundError(const char& m) : PacManError(m) {};
};
