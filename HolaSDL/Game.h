﻿#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "GameMAP.h"
#include "PacMan.h"
#include "Fantasma.h"
#include <iostream>
using namespace std;

struct Par
{
	int x, y;
};

//clase Game que controla todo el juego
class Game
{
private:
	SDL_Window*	window = nullptr;//puntero a ventana que creara al inicio del juego
	SDL_Renderer* renderer = nullptr;//lo mismo con un puntero a un renderer
	SDL_Event event;//aqui se guardan los eventos que ocurren en el juego
	int winWidth = 800;//tama�o de la pantalla
	int winHeight = 600;
	int winX = SDL_WINDOWPOS_CENTERED;//posicion de la pantalla
	int winY = SDL_WINDOWPOS_CENTERED;
	const int TAM = 20;//tamaño al que se dibujara el juego
	bool exit = false;//booleano que indica cuando se sale del juego (bucle ppal.)
	bool error = false;//informa de posibles errores
	bool gameOver = false, win = false;//ganar o perder
	int numComida = 0, score = 0;//numero de comida del mapa
	int fils, cols;//filas y columnas del tablero
	int nivel = 1;//nivel actual
	int Temp = 0;//temporizador
	bool temporizador = false;//temporizador, indica si podemos comer fantasmas
	GameMAP* gameMap;//tablero
	PacMan pacman;//jugador
	Fantasma fantasmas[4];//array con todos los fantasmas
	Texture* textures[8];//punteros a textura que contiene todos los sprites del juego
public:
	Game();//carga la ventana y el tablero del juego
	void menu();//menu principal del juego
	void run(int menu);//bucle ppal., dentro de el se ejecutan los siguientes metodos
	void render();//manda a cada una de las entidades que se pinten
	void update();//manda a cada una de las entidades que actualicen su posicion
	void handleEvents();//mira los eventos que ocurren en pantalla
	Texture* getTexture(int i);//proporciona la textura
	SDL_Renderer* getRenderer();//proporciona el renderer
	void setCell(int fils, int cols, MapCell tipoCasilla);//modifica una casilla del tablero
	void setComida(int i);//modifica el numero de comida restante
	void leeArchivo(string filename);//lee de un archivo un mapa y actualiza las casillas del tableros
	int getTabFils();//devuelve filas y columnas del tablero
	int getTabCols();
	void colisiones();
	Fantasma getFantasmas(int i);//devuelve un fantasma
	void muereFantasma(int i);//manda al fantasma i morirse
	void fantasmasComibles(bool sonComibles);//establece todos los fantasmas a comibles o no comibles
	PacMan getPacman();//devuelve a pacman
	void renderHud();//pinta el numero de vidas
	void addScore(int ascore);
	void GameOver();//termina el juego
	int getTam();//devuelve el tamaño del juego
	MapCell nextCell(int posX, int posY, int dirX, int dirY);//devuelve el tipo de la casilla contigua en la direccion dada
	void guardarPartida();
	void cargarPartida();
	~Game();//cierra la ventana etc. (acaba el juego)
};

