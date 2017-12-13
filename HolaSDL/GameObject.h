#pragma once
class Game;
#include "Texture.h"
#include <iostream>
using namespace std;

class GameObject {
protected:
	Game* game;
	Texture* texture = nullptr;//puntero a su textura. La carga Game y luego se la pasa 
	SDL_Renderer* renderer = nullptr;//puntero a renderer, Game se lo pasa
	int fil, col;//informacion de filas y columnas
public:
	GameObject() : fil(0), col(0) {};
	GameObject(Game* game) : game(game) {};
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void loadFromFile() = 0;
	virtual void saveToFile() = 0;
};

