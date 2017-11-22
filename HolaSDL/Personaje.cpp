#include "Personaje.h"
#include "Game.h"

//constructora por defecto
Personaje::Personaje()
{
	destRect.w = destRect.h = destRect.x = destRect.y = dirX = dirY = fil = col = numTicks = numFrames = 0;
}

//constructora que establece todos los atributos de la entidad
Personaje::Personaje(Game* game, int width, int height, int f, int c, int numT, int numF)
{
	this->game = game;
	renderer = game->getRenderer();
	destRect.w = width;//tamaño de pacman
	destRect.h = height;
	dirX = 0;
	dirY = 0;
	fil = f;
	col = c;
	numTicks = numT;
	numFrames = numF;
	dirXSig = 0;
	dirYSig = 0;
}

//manda a la textura de la entidad que pinte un frame a elegir
void Personaje::render()
{
	texture->renderFrame(renderer, destRect, fil, col);
}

//manda a la textura de la entidad que se anime
void Personaje::animate()
{
	texture->animation(renderer, destRect, angle, numTicks, numFrames);
}

//actualiza la posicion de la entidad
void Personaje::update()
{
	toroide();//miramos si se sale del mapa
}

//controla cuando la entidad se sale de los bordes del mapa
void Personaje::toroide()
{
	if (destRect.x >= (game->getTabCols()) * game->getTam())destRect.x = 0;//si salimos por la derecha entramos por la izquierda
	else if (destRect.x < 0)destRect.x = game->getTabCols() * game->getTam() - game->getTam();//y viceversa
	if (destRect.y >= (game->getTabFils()) * game->getTam())destRect.y = 0;//si salimos por abajo entramos por arriba
	else if (destRect.y < 0)destRect.y = game->getTabFils() * game->getTam() - game->getTam();//y viceversa
}

//llamado cuando perdemos una vida
void Personaje::morir()
{
		dirX = dirY = dirXSig = dirYSig = 0;
		destRect.x = posIniX;
		destRect.y = posIniY;
}

//establece la siguiente direccion a tomar (la tomaremos cuando podamos hacerlo)
void Personaje::siguienteDir(int newDirX, int newDirY)
{
	dirXSig = newDirX;
	dirYSig = newDirY;
}

void Personaje::Gira(double ang) { angle = ang; }

//establece la posicion de la entidad
void Personaje::setPos(int posY, int posX)
{
	posIniX = destRect.x = posX;
	posIniY = destRect.y = posY;
}

//devuelven posicion de la entidad
int Personaje::getPosX() { return destRect.x; }

int Personaje::getPosY() { return destRect.y; }

//devuelven posicion inicial de la entidad
int Personaje::getPosIniX() { return posIniX; }

int Personaje::getPosIniY() { return posIniY; }

//devuelven direccion de la entidad
int Personaje::getDirX() { return dirX; }

int Personaje::getDirY() { return dirY; }
