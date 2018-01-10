#pragma once
#include "PacManError.h"
#include <stdexcept>
#include <iostream>

using namespace std;

class FileFormatError : public PacManError {
public:
	FileFormatError(const char& m) : PacManError(m) {};
};
