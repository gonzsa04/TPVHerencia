#include "PacMan.h"
#include "Game.h"

PacMan::PacMan(Game* game, int width, int height, int f, int c, int numT, int numF) :
	Personaje(game, width, height, f, c, numT, numF) {
	texture = game->getTexture(1);
	vidas = 3;
}

//actualiza la posicion de pacman a traves de sus direcciones actual y siguiente
void PacMan::update()
{
	//si no hay un muro en la direccion siguiente, toma esa nueva direccion
	if (game->nextCell(destRect.x, destRect.y, dirXSig, dirYSig) != muro)
	{
		dirX = dirXSig;
		dirY = dirYSig;
		if (dirX < 0)Gira(180);//giramos a pacman en la direccion correspondiente
		else if (dirX > 0)Gira(0);
		else if (dirY < 0)Gira(270);
		else if (dirY > 0)Gira(90);
	}
	//si no hay un muro en la direccion actual, avanzamos en esa direccion
	if (game->nextCell(destRect.x, destRect.y, dirX, dirY) != muro)
	{
		//si hay comida o vitamina nos las comemos y sumamos 10 a la puntuacion
		if (game->nextCell(destRect.x, destRect.y, dirX, dirY) == comida
			|| game->nextCell(destRect.x, destRect.y, dirX, dirY) == vitamina)
		{
			//si es una vitamina ponemos todos los fantasmas a comible
			if (game->nextCell(destRect.x, destRect.y, dirX, dirY) == vitamina)game->fantasmasComibles(true);
			game->setCell((destRect.x + dirX) / game->getTam(), (destRect.y + dirY) / game->getTam(), vacio);
			game->setComida(-1);
			game->addScore(10);
		}
		destRect.x += dirX;
		destRect.y += dirY;
	}
	Personaje::update();
}

//llamado cuando perdemos una vida
void PacMan::morir()
{
	vidas--;//nos resta una vida y nos lleva a la posicion inicial
	if (vidas <= 0) game->GameOver();//si nos quedamos sin vidas perdemos
	else Personaje::morir();
}


//devuelve el numero de vidas restantes
int PacMan::getVidas() { return vidas; }
