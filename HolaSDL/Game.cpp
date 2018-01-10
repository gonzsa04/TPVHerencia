#include <fstream>
#include <string>
#include "Game.h"
#include <time.h>
#include <iostream>
#include <iterator>
#include "SDLError.h"
#include "FileNotFoundError.h"
#include "FileFormatError.h"

//inicializa la ventana del juego y todas las entidades
Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);//iniciamos SDL
	//damos a la ventana un nombre, con la posicion y el tamaño establecidos en Game
	window = SDL_CreateWindow("PACMAN", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	//le damos valor al renderer a partir de la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//avisamos si hay error
	if (window == nullptr || renderer == nullptr) throw SDLError(*SDL_GetError());
	//si no hay error
	else
	{
		srand((int)time(nullptr));
		for (int i = 0; i < numTextures; i++) { textures[i] = new Texture; }//array con todas las texturas del juego
		textures[0]->load(renderer, "..//images/pacman-spritesheet.png", 8, 4);//texturas de tablero
		textures[1]->load(renderer, "..//images/PacmanAnimation.png", 1, 4);//texturas de pacman
		textures[2]->load(renderer, "..//images/FAsustadosAnimation.png", 1, 2);//texturas de los fantasmas comibles
		textures[3]->load(renderer, "..//images/YouWin.png", 1, 1);//textura de ganar
		textures[4]->load(renderer, "..//images/Game-Over.png", 1, 1);//textura de perder
		textures[5]->load(renderer, "..//images/Menu1.png", 1, 1);//textura de menu modo jugar
		textures[6]->load(renderer, "..//images/score.png", 2, 6);//texturas de la puntuacion
		textures[7]->load(renderer, "..//images/Menu2.png", 1, 1);//textura de menu modo jugar
		int i = 0;//detectamos si hay algún error en la carga de imágenes
		while (i < numTextures && textures[i] == nullptr)i++;
		if (i < numTextures)throw SDLError(*IMG_GetError());

		characters.push_back(new PacMan(this, textures[1], TAM, TAM, 6, 2, 12, 4));//creamos a pacman
	}
}



//------------------------------------------------------------------JUEGO------------------------------------------------------------------
//menu principal del juego
void Game::Menu()
{
	//mientras no se haya pulsado salir
	int x, y, m = 5;
	while (!exit)
	{
		textures[m]->render(renderer);//se pinta el menu;
		SDL_GetMouseState(&x, &y);//si se ha pulsado jugar se carga el primer nivel
		if (x >= 370 && x <= 435 && y >= 300 && y <= 315) {
			m = 5;
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				leeArchivo("level0" + std::to_string(nivel) + ".pac");
				menu = 3;
			}
		}
		//si se ha pulsado cargar se carga el nivel seleccionado
		else if(x >= 370 && x <= 435 && y >= 335 && y <= 350){
			m = 7;
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				loadState = true;
				LoadState();
			}
		}
		SDL_RenderPresent(renderer);//representa (pinta todo)
		SDL_PollEvent(&event);//miramos los eventos
		run(menu);//bucle ppal

		SDL_RenderClear(renderer);
		//si hemos perdido lo mostramos por pantalla
		if (menu == 3)
		{ 
			menu = 1;
			nivel = 1;
			score = 0;
			if (gameOver) 
			{
				gameOver = false;
				textures[4]->render(renderer);
			}
			else if (win)
			{
				win = false;
				textures[3]->render(renderer);
			}
			SDL_RenderPresent(renderer);
			SDL_Delay(2000);//salimos al menu tras un delay
		}
		//salir ponemos el bool a true para salir del bucle ppal.
		if (event.type == SDL_QUIT)exit = true;
	}
}

//bucle principal
void Game::run(int menu) 
{
	while (!exit && !win && !gameOver && menu == 3)
	{
		handleEvents();//miramos los eventos que ocurran en pantalla
		colisiones();//controlamos colisiones entre pacman y fantasmas
		update();//mandamos a las entidades que actualicen su posicion
		render();//mandamos a las entidades que se pinten
		auxiliares();
	}
}

//quita peso de encima a run(), y lleva a cabo labores como gestionar
//el framerate y controlar cuando se pasa de nivel
void Game::auxiliares() 
{
	startTime = SDL_GetTicks();
	if (numComida == 0)
		win = true;//si nos comemos la comida ganamos

				   //temporiza el tiempo que puedes comerte a los fantasmas
	if (temporizador)Temp++;
	if (Temp >= 50) { Temp = 0; fantasmasComibles(false); }

	//framerate
	frameTime = SDL_GetTicks() - startTime;
	if (frameTime < 120)
		SDL_Delay(180 - frameTime);

	if (win)
	{
		for (int i = 0; i < numFantasmas; i++) characters.pop_front();
		if (nivel < numNiveles) {
			nivel++;
			leeArchivo("level0" + std::to_string(nivel) + ".pac");
			win = false;
		}
	}
	if (saveState)SaveState();

	if (event.type == SDL_QUIT)
	{
		for (int i = 0; i < numFantasmas; i++) characters.pop_front();
		exit = true;
	}
}

//mira los eventos que ocurran en el juego
void Game::handleEvents()
{
	while (SDL_PollEvent(&event) && !exit) {
		//salir ponemos el bool a true para salir del bucle ppal.
		if (event.type == SDL_QUIT)exit = true;
		else if (event.type == SDL_KEYDOWN)
		{//dependiendo de la tecla pulsada establecemos la siguiente direccion de pacman
			if (event.key.keysym.sym == SDLK_LEFT) (*characters.rbegin())->siguienteDir(-TAM, 0);
			else if (event.key.keysym.sym == SDLK_RIGHT) (*characters.rbegin())->siguienteDir(TAM, 0);
			else if (event.key.keysym.sym == SDLK_UP) (*characters.rbegin())->siguienteDir(0, -TAM);
			else if (event.key.keysym.sym == SDLK_DOWN) (*characters.rbegin())->siguienteDir(0, TAM);
			else if (event.key.keysym.sym == SDLK_s) saveState = true;//si pulsas s guardas partida
		}
	}
}

//manda a cada una de las entidades del juego que actualicen su posicion
void Game::update()
{
	for (list<Personaje*>::iterator it = characters.begin(); it != characters.end(); it++)
		(*it)->update();
}
//--------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------COLISIONES----------------------------------------------------------
void Game::colisiones()
{	
	list<Personaje*>::reverse_iterator it = characters.rbegin();
	it++;
	//colisiona con el si sus posiciones son iguales o si su siguiente posicion es igual que la del fantasma y la siguiente posicion del fantasma es la misma que la suya
	while (it != characters.rend() && ((*characters.rbegin())->getPosX() != (*it)->getPosX() || (*characters.rbegin())->getPosY() != (*it)->getPosY()) &&
		((*characters.rbegin())->getPosX() + (*characters.rbegin())->getDirX() != (*it)->getPosX() || (*characters.rbegin())->getPosY() +
		(*characters.rbegin())->getDirY() != (*it)->getPosY()))
		it++;
	//si hay colision 
	if (it != characters.rend())
	{
		//si no son comibles
		if (!static_cast<Fantasma*>(*it)->getComible())
		{
			//si es un fantasma listo y esta muerto le comemos y sumamos 200 puntos
			if (static_cast<Fantasma*>(*it)->esListo() && static_cast<SmartGhost*>(*it)->estaMuerto())
			{
				(*it)->morir();
				addScore(200);
			}
			//si no es listo o si es listo pero esta vivo se pierde una vida y tanto pacman como los fantasmas vuelven a su posicion inicial
			else {
				list<Personaje*>::iterator it2 = characters.begin();
				while (it2 != characters.end()) {
					(*it2)->morir();
					it2++;
				}
			}
		}
		//si son comibles te comes a ese fantasma y sumas 100 puntos
		else 
		{
			(*it)->morir();
			addScore(100);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------PINTADO------------------------------------------------------------
//manda a cada una de las entidades que se pinten
void Game::render()
{
	list<Personaje*>::reverse_iterator it = characters.rbegin();
	SDL_RenderClear(renderer);//borra
	gameMap->render();//le mandamos al tablero que se pinte a un tamaño
	(*characters.rbegin())->animate();//animamos pacman
	for(it++; it != characters.rend(); it++)(*it)->render();//pintamos los fantasmas
	renderHud();//pinta el HUD
	SDL_RenderPresent(renderer);//representa (pinta todo)
}

//pinta las vidas y la puntuacion
void Game::renderHud()
{
	SDL_Rect destRect;
	destRect.w = destRect.h = TAM;
	destRect.x = cols*TAM;
	destRect.y = 0;
	//pintamos las vidas
	for (int i = 0; i < static_cast<PacMan*>(*characters.rbegin())->getVidas(); i++) {
		destRect.x += TAM;
		textures[0]->renderFrame(renderer, destRect, 6, 2);
	}
	destRect.w = TAM * 4;
	destRect.x -= TAM * 2;
	destRect.y = TAM * 2;
	textures[6]->renderFrame(renderer, destRect, 0, 0);
	string sScore = std::to_string(score);
	destRect.x += 4 * TAM;
	//pintamos puntuacion
	for (int i = 0; i < (int)sScore.length(); i++)
	{
		destRect.x += 3*TAM/4;
		if(sScore[i] == '0'){ textures[6]->renderFrame(renderer, destRect, 0, 1); }
		else if (sScore[i] == '1') { textures[6]->renderFrame(renderer, destRect, 0, 2); }
		else if (sScore[i] == '2') { textures[6]->renderFrame(renderer, destRect, 0, 3); }
		else if (sScore[i] == '3') { textures[6]->renderFrame(renderer, destRect, 0, 4); }
		else if (sScore[i] == '4') { textures[6]->renderFrame(renderer, destRect, 1, 0); }
		else if (sScore[i] == '5') { textures[6]->renderFrame(renderer, destRect, 1, 1); }
		else if (sScore[i] == '6') { textures[6]->renderFrame(renderer, destRect, 1, 2); }
		else if (sScore[i] == '7') { textures[6]->renderFrame(renderer, destRect, 1, 3); }
		else if (sScore[i] == '8') { textures[6]->renderFrame(renderer, destRect, 1, 4); }
		else if (sScore[i] == '9') { textures[6]->renderFrame(renderer, destRect, 1, 5); }
	}
}
//-------------------------------------------------------------------------------------------------------



//-------------------------------------------GUARDAR/CARGAR----------------------------------------------
void Game::SaveState()
{
	SDL_Event evento;
	code = -1;
	while (saveState && !exit)
	{
		SDL_WaitEvent(&evento);
		if (evento.type == SDL_QUIT) exit = true;
		else if (evento.key.keysym.sym == SDLK_RETURN)
		{
			saveState = false;
			guardarPartida();
		}
		else if (evento.key.keysym.sym >= SDLK_0 && evento.key.keysym.sym <= SDLK_9)
			code = 10 * code + evento.key.keysym.sym - SDLK_0;
	}
}

void Game::LoadState()
{
	SDL_Event evento;
	localCode = -1;
	ifstream archivo;
	while (loadState && !exit)
	{
		while (SDL_PollEvent(&evento) && loadState)
		{
			if (evento.type == SDL_QUIT) exit = true;
			else if (evento.key.keysym.sym == SDLK_RETURN)
			{
				archivo.open("./Levels/NivelGuardado" + to_string(localCode));
				if (archivo.is_open()) {
					menu = 3;
					cargarPartida();
				}
				else 
				{
					leeArchivo("level0" + std::to_string(nivel) + ".pac");
					menu = 3;
				}
				loadState = false;
			}

			else if (evento.key.keysym.sym >= SDLK_0 && evento.key.keysym.sym <= SDLK_9)
				localCode = 10 * localCode + evento.key.keysym.sym - SDLK_0;
		}
	}
}

//guarda partida escribiendo la situacion de la misma en archivo de texto
void Game::guardarPartida()
{
	ofstream archivo;
	archivo.open("./Levels/NivelGuardado" + to_string(code));
	archivo << fils << " " << cols << endl;
	gameMap->saveToFile(archivo);
	archivo << numFantasmas << endl;

	list<Personaje*>::iterator it = characters.begin();
	while (it != characters.end()) {
		(*it)->saveToFile(archivo);//lo cargamos de fichero
		archivo << endl;
		it++;
	}
	archivo << nivel << " " << score;
	archivo.close();
}

//carga una partida guardada en un archivo de texto
void Game::cargarPartida()
{
	leeArchivo("NivelGuardado" + to_string(localCode));
}

//lee de archivo un mapa y modifica el array de casillas para que sea igual
void Game::leeArchivo(string filename)
{
	ifstream archivo;
	numComida = 0;
	archivo.open("./Levels/" + filename);
	if (archivo.is_open())
	{
		archivo >> fils >> cols;

		gameMap = new GameMAP(fils, cols, this);//creamos el tablero
		gameMap->loadFromFile(archivo);//cargamos el tablero con lo que hay en el fichero

		archivo >> numFantasmas;//leemos numero de fantasmas
		int smart;
		Personaje* ghost;
		for (int i = 0; i < numFantasmas; i++)
		{
			archivo >> smart;
			if (smart == 0) ghost = new Fantasma(this, textures[0], textures[2], TAM, TAM, i, 0, 1, 2);
			else ghost = new SmartGhost(this, textures[0], textures[2], TAM, TAM, 4, 0, 1, 2);
			//metemos a los fantasmas en el cjto. de personajes y los cargamos de fichero
			ghost->loadFromFile(archivo);
			characters.push_front(ghost);
		}
		
		(*characters.rbegin())->loadFromFile(archivo);//lo cargamos de fichero

		archivo >> nivel;
		archivo >> score;
		archivo.close();
	}
}
//----------------------------------------------------------------------------------------------



//-------------------------------------------AUXILIARES-----------------------------------------
//devuelve el tipo de la casilla contigua en la direccion dada
MapCell Game::nextCell(int posX, int posY, int dirX, int dirY)
{
	return(gameMap->getCell((posY + dirY) / TAM, (posX + dirX) / TAM));
}

//suma o resta uno a la comida (1 o -1 como parametros)
void Game::setComida(int i)
{
	numComida += i;
}

//devuelve la textura
Texture* Game::getTexture(int i) { return textures[i]; }

//devuelve el renderer
SDL_Renderer* Game::getRenderer() { return renderer; }

//establece una casilla con un valor dado
void Game::setCell(int fils, int cols, MapCell tipoCasilla)
{
	gameMap->setCell(fils, cols, tipoCasilla);
}

int Game::getTabFils() { return fils; }

int Game::getTabCols() { return cols; }

Personaje* Game::hayFantasma(int x, int y) { 
	list<Personaje*>::reverse_iterator it = characters.rbegin();
	it++;
	while (it != characters.rend() && ((*it)->getPosX() != x || (*it)->getPosY() != y)) it++;
	if (it != characters.rend())return *it;
	else return nullptr;
}//devuelve el fantasma si hay alguno en la posicion x, y

//establece todos los fantasmas a comibles o no comibles
void Game::fantasmasComibles(bool sonComibles)
{
	list<Personaje*>::reverse_iterator it = characters.rbegin();
	for(it++;it != characters.rend(); it++) static_cast<Fantasma*>(*it)->modifyComible(sonComibles);
	if (sonComibles) { temporizador = true; Temp = 0; }//si son comibles inicia el temporizador
	else temporizador = false;//si dejan de serlo finaliza el temporizador
}

Personaje* Game::getPacman() { return (*characters.rbegin()); }

int Game::getTam() { return TAM; }

void Game::GameOver()
{ 
	for (int i = 0; i < numFantasmas; i++) characters.pop_front();
	gameOver = true;
}

//añade puntos, llamado al comer comida vitaminas o fantasmas
void Game::addScore(int ascore)
{
	score += ascore;
}

int Game::numFant() { return numFantasmas; }

//spawnea un nuevo fantasma listo (llamado cuando dos de ellos se reproducen)
void Game::spawnFantasma(int x, int y) 
{
	SmartGhost* hijo = new SmartGhost(this, textures[0], textures[2], TAM, TAM, 4, 0, 1, 2);
	characters.push_front(hijo);//creamos un nuevo fantasma listo en la lista de personajes
	static_cast<Fantasma*>(*characters.begin())->setPos(y, x);//lo situamos en el lugar de sus padres
	numFantasmas++;//el numero de fantasmas ahora sera 1 mas
}
//---------------------------------------------------------------------------------------------------------



//-------------------------------------------DESTRUCTORA---------------------------------------------------
//finaliza el juego
Game::~Game()
{
	delete(gameMap);
	if (textures != nullptr) {
		for (int i = 0; i < numTextures; i++)delete[] textures[i];
	}
	list<Personaje*>::iterator it = characters.begin();
	while (it != characters.end()) {
		delete (*it);
		it++;
	}

	SDL_DestroyRenderer(renderer);//destruimos el renderer
	SDL_DestroyWindow(window);//destruimos la ventana
	SDL_Quit();
}