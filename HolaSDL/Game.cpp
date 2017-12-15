#include <fstream>
#include <string>
#include "Game.h"
#include <time.h>
#include <iostream>

//inicializa la ventana del juego y todas las entidades
Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);//iniciamos SDL
	//damos a la ventana un nombre, con la posicion y el tamaño establecidos en Game
	window = SDL_CreateWindow("PACMAN", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	//le damos valor al renderer a partir de la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//avisamos si hay error
	if (window == nullptr || renderer == nullptr)cout << "Error initializing SDL\n";
	//si no hay error
	else
	{
		srand((int)time(nullptr));
		for (int i = 0; i < 8; i++) { textures[i] = new Texture; }//array con todas las texturas del juego
		textures[0]->load(renderer, "..//images/pacman-spritesheet.png", 8, 4);//texturas de tablero
		textures[1]->load(renderer, "..//images/PacmanAnimation.png", 1, 4);//texturas de pacman
		textures[2]->load(renderer, "..//images/FAsustadosAnimation.png", 1, 2);//texturas de los fantasmas comibles
		textures[3]->load(renderer, "..//images/YouWin.png", 1, 1);//textura de ganar
		textures[4]->load(renderer, "..//images/Game-Over.png", 1, 1);//textura de perder
		textures[5]->load(renderer, "..//images/Menu1.png", 1, 1);//textura de menu modo jugar
		textures[6]->load(renderer, "..//images/Menu2.png", 1, 1);//textura de menu modo salir
		textures[7]->load(renderer, "..//images/score.png", 2, 6);//texturas de la puntuacion
	}
}

//devuelve el tipo de la casilla contigua en la direccion dada
MapCell Game::nextCell(int posX, int posY, int dirX, int dirY)
{
	return(gameMap->getCell((posY + dirY) / TAM, (posX + dirX) / TAM));
}

//menu principal del juego
void Game::Menu()
{
	//mientras no se haya pulsado salir
	while (!exit)
	{
		if (event.key.keysym.sym == SDLK_UP) menu = 1;//si estamos sobre la opcion jugar
		else if (event.key.keysym.sym == SDLK_DOWN) menu = 2;//si estamos sobre la opcion salir
		if (menu == 1) 
		{ 
			//si estamos sobre la opcion jugar representamos en pantalla con una flecha
			textures[5]->render(renderer); //si pulsamos espacio jugamos
			if (event.key.keysym.sym == SDLK_SPACE) 
			{
				leeArchivo("level0" + std::to_string(nivel) + ".pac");
				menu = 3; 
			}
		}
		else
		{
			//si estamos sobre la opcion salir representamos en pantalla con una flecha
			textures[6]->render(renderer);//si pulsamos espacio salimos
			if (event.key.keysym.sym == SDLK_SPACE)
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

	if (win && nivel < numNiveles)
	{
		nivel++;
		leeArchivo("level0" + std::to_string(nivel) + ".pac");
		win = false;
	}
	if (saveState)SaveState();
	else if (loadState)LoadState();

	if (event.type == SDL_QUIT)exit = true;
}

//mira los eventos que ocurran en el juego
void Game::handleEvents()
{
	while (SDL_PollEvent(&event) && !exit) {
		//salir ponemos el bool a true para salir del bucle ppal.
		if (event.type == SDL_QUIT)exit = true;
		else if (event.type == SDL_KEYDOWN)
		{//dependiendo de la tecla pulsada establecemos la siguiente direccion de pacman
			if (event.key.keysym.sym == SDLK_LEFT) characters[numFantasmas]->siguienteDir(-TAM, 0);
			else if (event.key.keysym.sym == SDLK_RIGHT) characters[numFantasmas]->siguienteDir(TAM, 0);
			else if (event.key.keysym.sym == SDLK_UP) characters[numFantasmas]->siguienteDir(0, -TAM);
			else if (event.key.keysym.sym == SDLK_DOWN) characters[numFantasmas]->siguienteDir(0, TAM);
			else if (event.key.keysym.sym == SDLK_s) saveState = true;//si pulsas s guardas partida
			else if (event.key.keysym.sym == SDLK_c) loadState = true;//si pulsas c cargas partida
		}
	}
}

//manda a cada una de las entidades del juego que actualicen su posicion
void Game::update()
{
	for (int i = 0; i < characters.size(); i++)characters[i]->update();
}

void Game::colisiones()
{	
	int i = 0;
	//colisiona con el si sus posiciones son iguales o si su siguiente posicion es igual que la del fantasma y la siguiente posicion del fantasma es la misma que la suya
	while (i < numFantasmas && (characters[numFantasmas]->getPosX() != characters[i]->getPosX() || characters[numFantasmas]->getPosY() != characters[i]->getPosY()) &&
		(characters[numFantasmas]->getPosX() + characters[numFantasmas]->getDirX() != characters[i]->getPosX() || characters[numFantasmas]->getPosY() +
			characters[numFantasmas]->getDirY() != characters[i]->getPosY()))
		i++;
	//si hay colision 
	if (i < numFantasmas)
	{
		//si no son comibles se pierde una vida y tanto pacman como los fantasmas vuelven a su posicion inicial
		if (!static_cast<Fantasma*>(getFantasmas(i))->getComible())
		{
			for (int i = 0; i < characters.size(); i++)characters[i]->morir();
		}
		//si son comibles te comes a ese fantasma y sumas 100 puntos
		else 
		{
			characters[i]->morir();
			addScore(100);
		}
	}
}

//manda a cada una de las entidades que se pinten
void Game::render()
{
	SDL_RenderClear(renderer);//borra
	gameMap->render();//le mandamos al tablero que se pinte a un tamaño
	characters[numFantasmas]->animate();//pinta entidades
	for (int i = 0; i < numFantasmas; i++) characters[i]->render();//pintamos los fantasmas
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
	for (int i = 0; i < static_cast<PacMan*>(characters[numFantasmas])->getVidas(); i++) {
		destRect.x += TAM;
		textures[0]->renderFrame(renderer, destRect, 6, 2);
	}
	destRect.w = TAM * 4;
	destRect.x -= TAM * 2;
	destRect.y = TAM * 2;
	textures[7]->renderFrame(renderer, destRect, 0, 0);
	string sScore = std::to_string(score);
	destRect.x += 4 * TAM;
	//pintamos puntuacion
	for (int i = 0; i < (int)sScore.length(); i++)
	{
		destRect.x += 3*TAM/4;
		if(sScore[i] == '0'){ textures[7]->renderFrame(renderer, destRect, 0, 1); }
		else if (sScore[i] == '1') { textures[7]->renderFrame(renderer, destRect, 0, 2); }
		else if (sScore[i] == '2') { textures[7]->renderFrame(renderer, destRect, 0, 3); }
		else if (sScore[i] == '3') { textures[7]->renderFrame(renderer, destRect, 0, 4); }
		else if (sScore[i] == '4') { textures[7]->renderFrame(renderer, destRect, 1, 0); }
		else if (sScore[i] == '5') { textures[7]->renderFrame(renderer, destRect, 1, 1); }
		else if (sScore[i] == '6') { textures[7]->renderFrame(renderer, destRect, 1, 2); }
		else if (sScore[i] == '7') { textures[7]->renderFrame(renderer, destRect, 1, 3); }
		else if (sScore[i] == '8') { textures[7]->renderFrame(renderer, destRect, 1, 4); }
		else if (sScore[i] == '9') { textures[7]->renderFrame(renderer, destRect, 1, 5); }
	}
}

void Game::SaveState()
{
	SDL_Event evento;
	code = 0;
	while(saveState && !exit)
	{
		while (SDL_PollEvent(&evento) && saveState)
		{
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
}

void Game::LoadState()
{
	SDL_Event evento;
	while (loadState && !exit)
	{
		while (SDL_PollEvent(&evento) && loadState)
		{
			if (evento.type == SDL_QUIT) exit = true;
			else if (evento.key.keysym.sym == SDLK_RETURN)
			{
				cout << code;
				cout << localCode;
				if (localCode == code)
				{
					menu = 3;
					cargarPartida();
				}
				loadState = false;
			}
			else if (evento.key.keysym.sym >= SDLK_0 && evento.key.keysym.sym <= SDLK_9)
				localCode = 10 * localCode + evento.key.keysym.sym - SDLK_0;
		}
	}
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
		characters.resize(numFantasmas + 1);
		for (int i = 0; i < numFantasmas; i++) 
		{
			characters[i] = new Fantasma(this, textures[0], textures[2], TAM, TAM, i, 0, 1, 2);
			characters[i]->loadFromFile(archivo);//metemos a los fantasmas en el cjto. de personajes y los cargamos de fichero
		}

		characters[numFantasmas] = new PacMan(this, textures[1], TAM, TAM, 6, 2, 12, 4);//creamos a pacman
		characters[numFantasmas]->loadFromFile(archivo);//lo cargamos de fichero

		archivo >> nivel;
		archivo >> score;
		archivo.close();
	}
}

int Game::getTabFils() { return fils; }

int Game::getTabCols() { return cols; }

Personaje* Game::getFantasmas(int i) { return characters[i]; }//devuelve el fantasma i

//establece todos los fantasmas a comibles o no comibles
void Game::fantasmasComibles(bool sonComibles)
{
	for (int i = 0; i < numFantasmas; i++)static_cast<Fantasma*>(characters[i])->modifyComible(sonComibles);
	if (sonComibles) { temporizador = true; Temp = 0; }//si son comibles inicia el temporizador
	else temporizador = false;//si dejan de serlo finaliza el temporizador
}

Personaje* Game::getPacman() { return characters[numFantasmas]; }

int Game::getTam() { return TAM; }

void Game::GameOver(){ gameOver = true; }

//añade puntos, llamado al comer comida vitaminas o fantasmas
void Game::addScore(int ascore)
{
	score += ascore;
}

//guarda partida escribiendo la situacion de la misma en archivo de texto
void Game::guardarPartida()
{
	ofstream archivo;
	archivo.open("./Levels/NivelGuardado");
	archivo << fils << " " << cols << endl;
	gameMap->saveToFile(archivo);
	archivo << numFantasmas << endl;

	for (int i = 0; i < characters.size(); i++){
		characters[i]->saveToFile(archivo);//lo cargamos de fichero
		archivo << endl;
		}
	archivo << nivel << " " << score;
	archivo.close();
}

//carga una partida guardada en un archivo de texto
void Game::cargarPartida() 
{
	leeArchivo("NivelGuardado");
}

//finaliza el juego
Game::~Game()
{
	delete(gameMap);
	if (textures != nullptr) {
		for (int i = 0; i < numTextures; i++)delete[] textures[i];
	}
	for (int i = 0; i < characters.size(); i++)delete[] characters[i];

	SDL_DestroyRenderer(renderer);//destruimos el renderer
	SDL_DestroyWindow(window);//destruimos la ventana
	SDL_Quit();
}