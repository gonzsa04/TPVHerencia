#include "SmartGhost.h"
#include "Game.h"
#include <fstream>
#include <cmath>

void SmartGhost::loadFromFile(ifstream& file)
{
	Fantasma::loadFromFile(file);//informacion de personaje
	file >> edad;
}

//guarda informacion sobre el fantasma en un fichero
void SmartGhost::saveToFile(ofstream& file)
{
	file << 1 << " ";
	Personaje::saveToFile(file);//informacion de personaje
	file << " " << edad;
}

void SmartGhost::update()
{
	posiblesDirecciones();//establecemos las posibles direcciones que puede tomar el fantasma

	bool encontrada = false;
	int i = 0;
	while (i < posiblesDir.size() && !encontrada) {
		dirX = posiblesDir[i].x;
		dirY = posiblesDir[i].y;
		if (abs((posiblesDir[i].x + destRect.x) - game->getPacman()->getPosX()) < abs(destRect.x - game->getPacman()->getPosX()) ||
			abs((posiblesDir[i].y + destRect.y) - game->getPacman()->getPosY()) < abs(destRect.y - game->getPacman()->getPosY())) {
			encontrada = true;
		}
		else i++;
	}
	destRect.x += dirX;
	destRect.y += dirY;

	Personaje::update();//miramos si se ha salido del mapa
}