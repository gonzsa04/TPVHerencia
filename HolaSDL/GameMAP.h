#pragma once
class Game;
#include "GameObject.h"

//enum con los tipos de casilla que puede tener el tablero
enum MapCell { vacio, muro, comida, vitamina };

//clase que representa el mapa o tablero del juego
//hereda de GameObject->el mapa ES una entidad
class GameMAP: public GameObject {
private:
	MapCell** cells;//array bidimensional de casillas
public:
	GameMAP(int fils, int cols, Game* g);//constructora "de verdad" que inicializa los parametros del tablero
	MapCell getCell(int f, int c);//devuelve una casilla en la posicion dada
	void setCell(int fils, int cols, MapCell tipoCasilla);//cambia la casilla dada por el valor correspondiente
	virtual void render();//pinta el tablero a un tamaño determinado
	virtual void update();//metodo que actualiza la posicion de la entidad
	virtual void loadFromFile(ifstream& file);
	virtual void saveToFile();
	~GameMAP();//destructora
};