#include <fstream>
#include "GameMAP.h"
#include "Game.h"

using namespace std;

//constructora "de verdad"
GameMAP::GameMAP(int fils, int cols, Game* game)
{
	//inicializa sus parametros
	this->fils = fils;
	this->cols = cols;
	this->game = game;
	cells = new MapCell*[fils];//da tamaño al array de casillas
	for (int i = 0; i < fils; i++)cells[i] = new MapCell[cols];
	textures = game->getTexture(0);//coge el renderer y la textura de game
	renderer = game->getRenderer();
}

//devuelve la casilla de la poscion dada
MapCell GameMAP::getCell(int f, int c)
{
	if (f < fils && c < cols && f >= 0 && c >= 0)
		return cells[f][c];
}

//cambia la casilla dada por el valor correspondiente
void GameMAP::setCell(int fils, int cols, MapCell tipoCasilla)
{
	cells[cols][fils] = tipoCasilla;
}

//pinta el mapa dado a un tamaño dado
void GameMAP::render(int tamanyo)
{
	//lee todas las casillas y pinta el sprite correspondiente de cada una
	for (int i = 0; i < fils; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (cells[i][j] == vacio)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 5, 3);
			}
			else if (cells[i][j] == muro)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 5, 0);
			}
			else if (cells[i][j] == comida)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 4, 2);
			}
			else if (cells[i][j] == vitamina)
			{
				SDL_Rect destRect;
				destRect.x = j*tamanyo;
				destRect.y = i*tamanyo;
				destRect.w = destRect.h = tamanyo;
				textures->renderFrame(renderer, destRect, 4, 3);
			}
		}
	}
}

//destructora del tablero
GameMAP::~GameMAP()
{
	if (cells != nullptr) {
		for (int i = 0; i < fils; i++)delete[] cells[i];
		delete[] cells;
	}
}