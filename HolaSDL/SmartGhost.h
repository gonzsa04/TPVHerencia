#pragma once
#include "Fantasma.h"

class SmartGhost : public Fantasma
{
private:
	int edad;
	int edadAd = 60;
	int edadMax = 250;
	bool adulto = false;
	bool muerto = false;
public:
	SmartGhost() :Fantasma() {};
	SmartGhost(Game* game, Texture* text1, Texture* text2, int width, int height, int f, int c, int numT, int numF) :
		Fantasma(game, text1, text2, width, height, f, c, numT, numF) {}
	virtual void loadFromFile(ifstream& file);
	virtual void saveToFile(ofstream& file);
	virtual void update();
	void envejece();
	void morir();
};