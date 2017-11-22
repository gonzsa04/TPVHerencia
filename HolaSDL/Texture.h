#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
using namespace std;

//clase textura que se encarga de pintar o animar a las entidades
class Texture
{
private:
	SDL_Texture* texture = nullptr;//puntero a textura
	int	w = 0;	//ancho de la textura
	int	h = 0;	//alto de la textura
	int fw = 0;	//ancho de cada frame
	int fh = 0;	//alto de cada frame
	SDL_Rect srcRect;//rectangulo fuente de la textura

public:
	bool load(SDL_Renderer*	renderer, string filename, int fils, int cols);//carga una textura de un png (lo hara Game)
	void render(SDL_Renderer* renderer);//pinta la textura (primer frame)
	void renderFrame(SDL_Renderer*renderer, const SDL_Rect& destRect, int fil, int col);//pinta el frame elegido
	void animation(SDL_Renderer* renderer, const SDL_Rect& destRect, double ang, int NumTicks, int NumFrames);
	//anima la textura en funcion de la velocidad y el numero de frames
};
