#include <fstream>
#include "GameMAP.h"
#include "Game.h"
#include <fstream>

using namespace std;

GameMAP::GameMAP(int fils, int cols, Game* g) : GameObject(g) {
	//inicializa sus parametros
	this->fil = fils;
	this->col = cols;
	cells = new MapCell*[fils];//da tamaño al array de casillas
	for (int i = 0; i < fils; i++)cells[i] = new MapCell[cols];
	texture = game->getTexture(0);//coge el renderer y la textura de game
	renderer = game->getRenderer();
};//constructora "de verdad" que inicializa los parametros del tablero

//devuelve la casilla de la poscion dada
MapCell GameMAP::getCell(int f, int c)
{
	if (f < fil && c < col && f >= 0 && c >= 0)
		return cells[f][c];
}

//cambia la casilla dada por el valor correspondiente
void GameMAP::setCell(int fils, int cols, MapCell tipoCasilla)
{
	cells[cols][fils] = tipoCasilla;
}

//pinta el mapa dado a un tamaño dado
void GameMAP::render()
{
	int tamanyo = game->getTam();
	//lee todas las casillas y pinta el sprite correspondiente de cada una
	for (int i = 0; i < fil; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (cells[i][j] == vacio)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				texture->renderFrame(renderer, destRect, 5, 3);
			}
			else if (cells[i][j] == muro)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				texture->renderFrame(renderer, destRect, 5, 0);
			}
			else if (cells[i][j] == comida)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				texture->renderFrame(renderer, destRect, 4, 2);
			}
			else if (cells[i][j] == vitamina)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				texture->renderFrame(renderer, destRect, 4, 3);
			}
		}
	}
}

void GameMAP::update(){}
void GameMAP::loadFromFile(ifstream& file)
{
	int tipoCasilla;
	for(int i=0;i<fil;i++)
		for (int j = 0; j < col; j++) 
		{
			file >> tipoCasilla;
			setCell(j, i, (MapCell)tipoCasilla);
			if (getCell(i, j) == comida || getCell(i, j) == vitamina) game->setComida(1);
		}
}

void GameMAP::saveToFile() {}

//destructora del tablero
GameMAP::~GameMAP()
{
	if (cells != nullptr) {
		for (int i = 0; i < fil; i++)delete[] cells[i];
		delete[] cells;
	}
}