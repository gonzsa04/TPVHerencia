#include "Fantasma.h"
#include "Game.h"

void Fantasma::render()
{
	if (!comible) Personaje::render();
	else animate();
}

//manda a la textura de Fantasma que se anime
void Fantasma::animate()
{
	texture2->animation(renderer, destRect, 0, numTicks, numFrames);
}

//actualiza la posicion de Fantasma a traves de sus direcciones actual y siguiente
void Fantasma::update()
{
	posiblesDirecciones();//establecemos las posibles direcciones que puede tomar el fantasma

	//escogemos una al azar y hacemos que vaya en esa direccion
	int randDir = rand() % posiblesDir.size();
	dirX = posiblesDir[randDir].x;
	dirY = posiblesDir[randDir].y;
	destRect.x += dirX;
	destRect.y += dirY;

	Personaje::update();//miramos si se ha salido del mapa
}

//llamado cuando pacman le come o cuando pacman pierde una vida
void Fantasma::morir()
{
	Personaje::morir();
	comible = false;//deja de ser comible
}

//elimina la direccion contraria a la que lleva de la lista de posibles direcciones
void Fantasma::eliminaDir(int x, int y) 
{
	int i = 0;
	if (x != 0)x = -x;
	else y = -y;
	while (i < (int)posiblesDir.size() - 1 && (posiblesDir[i].x != x || posiblesDir[i].y != y))i++;
	posiblesDir.erase(posiblesDir.begin() + i);
}

//indica si hay fantasma o no en la siguiente casilla donde va a avanzar el fantasma
bool Fantasma::hayFantasma(int dX, int dY) 
{
	int i = 0;
	while (i < 4 && (game->getFantasmas(i).destRect.x != destRect.x + dX || 
		game->getFantasmas(i).destRect.y != destRect.y + dY)) i++;
	return (i < 4);
}

//rellena un vector con todas las posibles direcciones que puede tomar el fantasma
void Fantasma::posiblesDirecciones() 
{
	int i = 0;
	posiblesDir.resize(1);
	if (game->nextCell(destRect.x, destRect.y, game->getTam(), 0) != muro && !hayFantasma(game->getTam(), 0))
	{
		//si hacia la derecha no hay muro y no hay fantasmas, se añade derecha como posible direccion
		posiblesDir[i].x = game->getTam();
		posiblesDir[i].y = 0;
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, -game->getTam(), 0) != muro && !hayFantasma(-game->getTam(), 0))
	{
		//igual con izquierda, arriba y abajo
		posiblesDir[i].x = -game->getTam();
		posiblesDir[i].y = 0;
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, 0, game->getTam()) != muro && !hayFantasma(0, game->getTam()))
	{
		posiblesDir[i].x = 0;
		posiblesDir[i].y = game->getTam();
		i++;
		posiblesDir.resize(i + 1);
	}
	if (game->nextCell(destRect.x, destRect.y, 0, -game->getTam()) != muro && !hayFantasma(0, -game->getTam()))
	{
		posiblesDir[i].x = 0;
		posiblesDir[i].y = -game->getTam();
		i++;
		posiblesDir.resize(i + 1);
	}

	if (i > 1) eliminaDir(dirX, dirY);//si hay mas de una posible direccion eliminamos la direccion opuesta a la que llevabamos
	//para que el fantasma no se de la vuelta sin motivo
	if (i != 0) { posiblesDir.erase(posiblesDir.begin() + posiblesDir.size() - 1); }//eliminamos la ultima direccion que estara vacia
	else posiblesDir[0].x = posiblesDir[0].y = 0;
}

//establece el fantasma a comible o no comible
void Fantasma::modifyComible(bool esComible)
{
	comible = esComible;
}

//devuelve si el fantasma es comible o no
bool Fantasma::getComible() { return comible; }