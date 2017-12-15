//practicando con SDL
#pragma once
class Game;
#include "SDL.h"
#include "SDL_image.h"
#include "GameObject.h"

//clase Personaje que rige el comportamiento del jugador y los fantasmas
//hereda de la clase GameObject->los personajes SON entidades
class Personaje: public GameObject {
protected:
	int posIniX, posIniY;//posicion inicial
	int dirX, dirY;//direccion actual
	int dirXSig, dirYSig;//siguiente direccion a tomar cuando sea posible
	double angle = 0;//angulo de giro
	int numTicks, numFrames;//velocidad de la animacion y numero de frames que tiene
	SDL_Rect destRect;//rectangulo destino donde se pintara el personaje
public:
	Personaje(): GameObject() {
		destRect.w = destRect.h = destRect.x = destRect.y = dirX = dirY = numTicks = numFrames = 0;
	};//constructora por defecto
	//constructora que inicializa todos los campos de la entidad vistos anteriormente
	Personaje(Game* g, int width, int height, int f, int c, int numT, int numF);
	virtual void render();//metodo que manda pintar a la entidad
	void Gira(double ang);
	virtual void animate();//metodo que manda animar a la entidad
	virtual void update();//metodo que actualiza la posicion de la entidad
	void toroide();//controla cuando la entidad se sale de los bordes del mapa
	virtual void morir();//metodo llamado al morir, devuelve a la entidad a posIni
	void siguienteDir(int newDirX, int newDirY);//establece la siguiente direccion de la entidad
	void setPos(int posY, int posX);//establece la posicion de la entidad
	virtual void loadFromFile(ifstream& file);//obtiene informacion sobre una entidad de un fichero
	virtual void saveToFile();//guarda informacion de una entidad en un fichero
	int getPosX();//devuelven posicion y direccion de la entidad
	int getPosY();
	int getPosIniX();
	int getPosIniY();
	int getDirX();
	int getDirY();
};

