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

void SmartGhost::envejece()
{
	if(edad == edadAd)
	{
		col = 1;
		adulto = true;
	}
	else if (edad >= edadMax)
	{
		muerto = true;
		fil = 5;
	}
	edad++;
}

void SmartGhost::morir()
{
	edad = 0;
	col = 0;
	fil = 4;
	adulto = false; 
	muerto = false;
	Fantasma::morir();
}

void SmartGhost::update()
{
	if (adulto) {
		posiblesDirecciones();//establecemos las posibles direcciones que puede tomar el fantasma

		bool encontrada = false;
		int i = 0;
		while (i < posiblesDir.size() && !encontrada && !muerto)
		{
			dirX = posiblesDir[i].x;
			dirY = posiblesDir[i].y;
			if (abs((dirX + destRect.x) - game->getPacman()->getPosX()) < abs(destRect.x - game->getPacman()->getPosX()) ||
				abs((dirY + destRect.y) - game->getPacman()->getPosY()) < abs(destRect.y - game->getPacman()->getPosY()))
				encontrada = true;
			i++;
		}
		if (!muerto)
		{
			destRect.x += dirX;
			destRect.y += dirY;
		}
		Personaje::update();//vemos si se sale del mapa
	}
	else Fantasma::update();
	envejece();
}