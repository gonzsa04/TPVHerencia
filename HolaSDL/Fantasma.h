//practicando con SDL
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Personaje.h"
#include "Texture.h"
class Game;
#include <iostream>
#include <vector>
using namespace std;

struct Punto 
{
	int x, y;
};

//clase Fantasma que rige el comportamiento de los enemigos
//hereda su comportamiento de Personaje, es un personaje
class Fantasma: public Personaje {
private:
	Texture* texture2 = nullptr;//puntero a su textura. La carga Game y luego se la pasa 
	vector<Punto>posiblesDir = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0} };//vector de posibles direcciones, inicialmente todas
	bool comible = false;//inicialmente el fantasma no es comible
	bool smart = false;
	int edad = 0;
public:
	Fantasma() :Personaje() {};//constructora que llama a la constructora por defecto de su padre
	//constructora, llama a la constructora del padre y ademas hace lo que esta entre {}
	Fantasma(Game* game, Texture* text1, Texture* text2, int width, int height, int f, int c, int numT, int numF) :
		Personaje(game, width, height, f, c, numT, numF) {
		texture = text1;
		texture2 = text2;
		Punto dirIni = posiblesDir[rand() % 4];
	}
	void loadFromFile(ifstream& file);
	void render();//metodo que manda pintar al fantasma
	void animate();//metodo que manda animar al fantasma
	void update();//metodo que actualiza la posicion del fantasma
	void morir();//metodo llamado al morir, devuelve al fantasma a posIni
	void eliminaDir(int x, int y);//elimina la direccion contraria a la que lleva el fantasma
	bool hayFantasma(int dX, int dY);//indica si hay fantasma o no en la siguiente casilla donde va a avanzar el fantasma
	void posiblesDirecciones();//rellena un vector con todas las posibles direcciones que puede tomar el fantasma
	void modifyComible(bool esComible);//cambia el fantasma a comible o no comible
	bool getComible();//devuelve si el fantasma es comible
};

