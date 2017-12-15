#pragma once
class Game;
#include "Texture.h"
#include <iostream>
using namespace std;

//clase GameObject, que comprende todas las entidades del juego
class GameObject {
protected:
	Game* game;//puntero al juego
	Texture* texture = nullptr;//puntero a la textura de la entidad
	SDL_Renderer* renderer = nullptr;//puntero a renderer
	int fil, col;//informacion de filas y columnas
public:
	GameObject() : fil(0), col(0) {};//constructora vacia
	GameObject(Game* g) : game(g) {};//constructora que inicializa game
	virtual void render() = 0;//render, update, loadFromFile y saveToFile
	virtual void update() = 0;//son metodos que tendran todas las entidades en comun
	virtual void loadFromFile(ifstream& file) = 0;//pero por ahora los dejamos vacios
	virtual void saveToFile() = 0;//para ser definidos despues
};

