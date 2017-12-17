#pragma once
#include "Fantasma.h"

//nueva clase de fantasma con nuevas caracteristicas
//hereda de Fantasma. ES un fantasma
class SmartGhost : public Fantasma
{
private:
	int edad;//edad actual
	int edadAdulta = 60;
	int edadMax = 200;//maxima edad, cuando llega a ella muere
	bool adulto = false;//si es adulto
	bool muerto = false;//si ha muerto
	bool haReproducido = false;//indica si se ha reproducido recientemente
public:
	SmartGhost() :Fantasma() {};//constructora por defecto y constructora de Fantasma
	SmartGhost(Game* game, Texture* text1, Texture* text2, int width, int height, int f, int c, int numT, int numF) :
		Fantasma(game, text1, text2, width, height, f, c, numT, numF) {smart = true;}
	virtual void loadFromFile(ifstream& file);//carga
	virtual void saveToFile(ofstream& file);//guarda
	virtual void update();//movimiento
	void persiguePacMan();
	void reproduccion();//hace que un fantasma listo se reproduzca con otro listo
	bool estaMuerto();//indica si el fantasma esta muerto
	void envejece();//se encarga de hacer envejecer al fantasma
	void morir();//le manda morir
	bool esAdulto();
};