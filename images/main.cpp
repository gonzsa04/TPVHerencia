//practicando con SDL
#include "SDL.h"
#include "SDL_image.h"
#include "Header.h"
#include "GameObject.h"
#include "Game.h"
#include <iostream>
using namespace std;


//----------------------------TEXTURE---------------------------------

//carga una textura de un png y establece las variables de la textura
bool Texture:: load(SDL_Renderer* renderer, string filename, int fils, int cols)
{
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface != nullptr){
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
void Texture::animation(SDL_Renderer* renderer, const SDL_Rect& destRect, int NumTicks, int NumFrames)
{
	srcRect.x = srcRect.w * int(((SDL_GetTicks() / NumTicks) % NumFrames));
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}



//--------------------------------------GAME OBJECT-----------------------------------------

//establece todos los atributos de cada entidad
void GameObject::GO(Texture* text, SDL_Renderer* renderer, int width, int height, int posX, int posY, int dX,
	int dY, int speed, int f, int c, int numT, int numF)
{
	texture = text;
	rend = renderer;
	destRect.w = width;
	destRect.h = height;
	destRect.x = posX;
	destRect.y = posY;
	dirX = dX;
	dirY = dY;
	Speed = speed;
	fil = f;
	col = c;
	numTicks = numT;
	numFrames = numF;
}

//manda a la textura de la entidad que pinte un frame a elegir
void GameObject::render() 
{
	texture->renderFrame(rend, destRect, fil, col);
}

//manda a la textura de la entidad que se anime
void GameObject::animate()
{
	texture->animation(rend, destRect, numTicks, numFrames);
}

//actualiza la posicion de la entidad a traves de su direccion y velocidad
void GameObject::update()
{
	destRect.x += dirX*Speed;
	if (dirX >= 0 && destRect.x > 800)destRect.x = 0;
	else if (destRect.x < 0)destRect.x = 800;
}

//muestra la velocidad de la entidad
int GameObject::viewSpeed()
{
	return Speed;
}

//modifica la velocidad de la entidad
void GameObject::modifySpeed(int newVelocity)
{
	Speed = newVelocity;
}




//--------------------------------------GAME-----------------------------------------

//inicializa la ventana del juego y todas las entidades
Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);//iniciamos SDL
	//damos a la ventana un nombre, con la posicion y el tamaño establecidos en Game
	window = SDL_CreateWindow("My first test wih SDL", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	//le damos valor al renderer a partir de la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//avisamos si hay error
	if (window == nullptr || renderer == nullptr)cout << "Error initializing SDL\n";
	//si no hay error
	else
	{
		for (int i = 0; i < 3; i++)textures[i] = new Texture;//creamos una nueva textura para cada entidad
		textures[0]->load(renderer, "..\\images\\background1.png", 1, 1);//cargamos cada textura de la lista
		textures[1]->load(renderer, "..\\images\\dog.png", 1, 6);//con la imagen correspondiente
		textures[2]->load(renderer, "..\\images\\helicopter.png", 1, 5);

		//inicializamos cada entidad del juego pasandole su textura y el renderer y estableciendo sus atributos
		backGround.GO(textures[0], renderer, 800, 600, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		dog.GO(textures[1], renderer, 100, 100, 0, 450, 1, 0, 1, 0, 0, 100, 6);
		helicopter.GO(textures[2], renderer, 100, 60, 600, 100, -1, 0, 1, 0, 0, 50, 5);
	}
}

//bucle principal del juego
void Game::run()
{
	//mientras no se haya pulsado salir
	while (!exit)
	{
		handleEvents();//miramos los eventos que ocurran en pantalla
		update();//mandamos a las entidades que actualicen su posicion
		render();//mandamos a las entidades que se pinten
		SDL_Delay(5);
	}
	finish();//al salir del bucle cerramos todo
}

//mira los eventos que ocurran en el juego
void Game::handleEvents()
{
	SDL_PollEvent(&event);//si se ha pulsado 
	//salir ponemos el bool a true para salir del bucle ppal.
	if (event.type == SDL_QUIT)exit = true;
	//si se ha pulsado una tecla
	else if (event.type == SDL_KEYDOWN) 
	{
		//si es 'd' paramos o reanudamos la marcha del perro
		if (event.key.keysym.sym == 'd')
		{
			if(dog.viewSpeed() != 0)dog.modifySpeed(0);
			else dog.modifySpeed(1);
		}
		//lo mismo con el helicoptero si pulsamos 'h'
		else if (event.key.keysym.sym == 'h') 
		{
			if (helicopter.viewSpeed() != 0)helicopter.modifySpeed(0);
			else helicopter.modifySpeed(1);
		}
		//si pulsamos 'f' aumentamos la velocidad del perro
		else if (event.key.keysym.sym == 'f') 
		{
			int newSpeed = dog.viewSpeed();
			dog.modifySpeed(newSpeed+=1);
		}
		//si pulsamos 's' disminuimos la velocidad del perro
		else if (event.key.keysym.sym == 's')
		{
			int newSpeed = dog.viewSpeed();
			if (dog.viewSpeed() != 0)dog.modifySpeed(newSpeed -= 1);
		}
		//lo mismo con el helicoptero si pulsamos 'j' o 'g'
		else if (event.key.keysym.sym == 'j')
		{
			int newSpeed = helicopter.viewSpeed();
			helicopter.modifySpeed(newSpeed += 1);
		}
		else if (event.key.keysym.sym == 'g')
		{
			int newSpeed = helicopter.viewSpeed();
			if (helicopter.viewSpeed() != 0)helicopter.modifySpeed(newSpeed -= 1);
		}
	}
}

//manda a cada una de las entidades del juego que actualicen su posicion
void Game::update()
{
	dog.update();
	helicopter.update();
}

//manda a cada una de las entidades que se pinten
void Game::render()
{
	SDL_RenderClear(renderer);//borra
	backGround.render();//pinta entidades
	dog.animate();
	helicopter.animate();
	SDL_RenderPresent(renderer);//representa (pinta todo)
}

//finaliza el juego
void Game::finish()
{
	SDL_DestroyRenderer(renderer);//destruimos el renderer
	SDL_DestroyWindow(window);//destruimos la ventana
	SDL_Quit();
}



//-----------------------------MAIN----------------------------
int main(int argc, char* argv[])
{
	Game game;//cargamos el juego (ventana, entidades, etc.)
	game.run();//bucle ppal.
	return 0;
}