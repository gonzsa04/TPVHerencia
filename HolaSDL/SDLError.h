#pragma once
#include "PacManError.h"
#include <stdexcept>
#include <iostream>

using namespace std;

class SDLError : public PacManError {
public:
	SDLError(const char& m) : PacManError(m) {};
};