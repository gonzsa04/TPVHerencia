#include "Texture.h"
using namespace std;

//carga una textura de un png y establece las variables de la textura
bool Texture::load(SDL_Renderer* renderer, string filename, int fils, int cols)
{
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		w = surface->w;
		h = surface->h;
		fw = w / cols;
		fh = h / fils;
		SDL_FreeSurface(surface);
		srcRect.w = fw;
		srcRect.h = fh;
		srcRect.x = srcRect.y = 0;
		return true;
	}
	else return false;
}

//pinta la textura (primer frame y en toda la pantalla)
void Texture::render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, &srcRect, nullptr);
}

//pinta el frame elegido
void Texture::renderFrame(SDL_Renderer*	renderer, const SDL_Rect& destRect, int	fil, int col)
{
	srcRect.x = fw*col;
	srcRect.y = fh*fil;
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

//anima la textura en funcion de la velocidad y el numero de frames
void Texture::animation(SDL_Renderer* renderer, const SDL_Rect& destRect, double ang, int NumTicks, int NumFrames)
{
	SDL_Point* centro = new SDL_Point;
	centro->x = destRect.w / 2;
	centro->y = destRect.h / 2;
	srcRect.x = srcRect.w * int(((SDL_GetTicks() / NumTicks) % NumFrames));
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, ang , centro, SDL_FLIP_NONE);
}