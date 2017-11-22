//practicando con SDL
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Personaje.h"
#include "Texture.h"
class Game;
#include <iostream>
using namespace std;

//clase PacMan que rige el comportamiento del jugador
//hereda sus comportamientos de Personaje, es un personaje
class PacMan: public Personaje {
private:
	int vidas;//numero de vidas
public:
	PacMan() :Personaje() {};//se llama a la constructora por defecto de su padre
	PacMan(Game* game, int width, int height, int f, int c, int numT, int numF);//constructora
	void update();//actualiza su posicion
	void morir();//metodo llamado al morir, resta una vida y devuelve a pacman a posIni
	int getVidas();//devuelve el numero de vidas que le quedan a pacman
};
