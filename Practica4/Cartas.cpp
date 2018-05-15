#include "checkML.h"
#include "Cartas.h"
#include <cstdlib> 
#include <algorithm>

void crearMazo(tMazo& mazo);

void crearVacia(tMazo& mazo) {
	mazo.inicio = 0;
	mazo.numCartas = 0;
}

bool sacar(tMazo& mazo, tCarta& carta) {
	bool sacar = false;
		if (mazo.numCartas != 0 ) {
			carta = *mazo.cartas[mazo.inicio];
			mazo.inicio++;
			mazo.numCartas--;
			mazo.inicio %= MAX_CARTAS;
			sacar = true;
		}
	return sacar;
}

void insertar(tMazo& mazo, tCarta carta) {
		*mazo.cartas[mazo.numCartas] = carta;
		mazo.numCartas++;
}

void crearMazoAleatorio(tMazo& mazo) {
	crearMazo(mazo);
	std::random_shuffle(&mazo.cartas[0], &mazo.cartas[MAX_CARTAS]);
}

void crearMazo(tMazo& mazo) { //Crea un mazo sin barajear
	//Reserva la memoria para las cartas de la baraja
	for (int i = 0; i < MAX_CARTAS; i++)
		mazo.cartas[i] = new tCarta;

	//A�ade las cartas al mazo
	for (int i = 0; i < MAX_AVANZAR; i++, mazo.numCartas++)
		*mazo.cartas[mazo.numCartas] = AVANZAR;

	for (int i = 0; i < MAX_LADO; i++, mazo.numCartas++)
		*mazo.cartas[mazo.numCartas] = GIRODERECHA;

	for (int i = 0; i < MAX_LADO; i++, mazo.numCartas++)
		*mazo.cartas[mazo.numCartas] = GIROIZQUIERDA;

	for (int i = 0; i < MAX_LASER; i++, mazo.numCartas++)
		*mazo.cartas[mazo.numCartas] = LASER;
}


void liberar(tMazo & mazo) {
	/*
	for (int i = 0; i < MAX_CARTAS; i++)
	{
		delete mazo.cartas[i];
	}
	*/
	delete[] mazo.cartas;
	mazo.inicio= 0;
	mazo.numCartas = 0;
}