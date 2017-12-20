#include "SmartGhost.h"
#include "Game.h"
#include <fstream>
#include <cmath>

//carga informacion sobre el fantasma de un fichero
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

//movimiento del fantasma listo
void SmartGhost::update()
{
	//si es adulto se comporta inteligentemente (busca a pacman)
	if(adulto) persiguePacMan();
	//si aun no es adulto se comporta como el resto de fantasmas
	else Fantasma::update();
	envejece();//le mandamos envejecer
	reproduccion();//mira si puede reproducirse
}

//hace que el fantasma persiga a pacman
void SmartGhost::persiguePacMan() 
{
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
	//si no ha muerto avanza en la direccion tomada
	if (!muerto)
	{
		destRect.x += dirX;
		destRect.y += dirY;
	}
	Personaje::update();//vemos si se sale del mapa
}

//hace que un fantasma listo se reproduzca con otro listo
void SmartGhost::reproduccion()
{
	//si el fantasma es adulto y no esta muerto
	if (adulto && !muerto) {
		Personaje* chocado = game->hayFantasma(destRect.x + dirX, destRect.y + dirY);
		if (chocado != nullptr) 
		{
			//si ha chocado con algun fantasma y este es listo, adulto, no esta muerto y no se ha reproducido aun
			if (static_cast<SmartGhost*>(chocado)->esListo() && static_cast<SmartGhost*>(chocado)->esAdulto()
				&& !static_cast<SmartGhost*>(chocado)->estaMuerto() && !haReproducido)
			{
				haReproducido = true;//marcamos como que ya se ha reproducido
				game->spawnFantasma(destRect.x, destRect.y);//spawneamos un nuevo fantasma listo en la posicion de uno de los padres
			}
		}
	}
}

//indica si esta muerto o no
bool SmartGhost::estaMuerto() { return muerto; }

//mata al fantasma
void SmartGhost::morir()
{
	edad = 0;
	col = 0;//cambiamos su sprite
	fil = 4;
	adulto = false;
	muerto = false;
	haReproducido = false;//puede volver a reproducirse
	Fantasma::morir();
}

//envejece al fantasma
void SmartGhost::envejece()
{
	//si alcanza la edad adulta se cambia de sprite (crece)
	if (edad == edadAdulta)
	{
		col = 1;
		adulto = true;
	}
	//si alcanza la maxima edad cambia de sprite y muere
	else if (edad >= edadMax)
	{
		muerto = true;
		fil = 5;
	}
	edad++;//va aumentando su edad
}

bool SmartGhost::esAdulto() { return adulto; }