#include "Dibujo.h"
#include <Windows.h>
#include <iostream>
#include <fstream>


void imprimirCasilla(const tTablero& tablero, int fila, int columna);
std::string dirJugador(const tTablero& tablero, int fila, int columna);

void setColor(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

void mostrarTablero(const tTablero& tablero) {
	system("cls");
	for (int i = 0; i < NUM_FILAS; i++) {
		for (int j = 0; j < NUM_COLUMNAS; j++) {
			imprimirCasilla(tablero, i , j);
		}
		std::cout << std::endl;
	}
}

void imprimirCasilla(const tTablero& tablero, int fila, int columna) {
	const tEstadoCasilla& casilla = tablero[fila][columna].estado;
	std::string imprimir;
	int numeroJugador = (tablero[fila][columna].tortuga.numero);

	switch (casilla) {
	case VACIA: setColor(PALETA[VACIA]); imprimir = "  "; break;
	case HIELO: setColor(PALETA[HIELO]); imprimir = "**"; break;
	case MURO: setColor(PALETA[MURO]); imprimir = "||"; break;
	case CAJA: setColor(PALETA[CAJA]); imprimir = "[]"; break;
	case JOYA: setColor(PALETA[JOYA]); imprimir = "00"; break;
	case TORTUGA: 
		setColor(PALETA[TORTUGA + numeroJugador]); 
		imprimir = dirJugador(tablero, fila, columna);
		break;
	}
	std::cout << imprimir;
	setColor(0);
	

}

std::string dirJugador(const tTablero& tablero, int fila, int columna) {
	std::string dir;

	tDir dirJug = tablero[fila][columna].tortuga.direccion;
	switch (dirJug) {
	case NORTE: dir = "^^"; break;
	case ESTE: dir = ">>"; break;
	case SUR: dir = "vv"; break;
	case OESTE: dir = "<<"; break;
	}
	return dir;
}

void mostrarJugadores(const tJuego& juego) {
	std::cout << "JUGADORES" << std::endl << std::endl;
	std::cout << "Turno de: " << juego.arrayJugadores[juego.turnoActual].nombre << std::endl << std::endl;
	for (int i = 0; i < juego.numJugadores; i++)
	{
		if (juego.turnoActual == i) {
			setColor(PALETA[TORTUGA + i]);
			std::cout <<  ">> " << i + 1 << ". " << juego.arrayJugadores[i].nombre << ":";
			setColor(0);
		}
		else {
			setColor(PALETA[TORTUGA + i]);
			std::cout  << "   " << i + 1 << ". " << juego.arrayJugadores[i].nombre << ":";
			setColor(0);
		}
		setColor(PALETA[9]);
		setColor(0); std::cout << " " << juego.arrayJugadores[i].mano.avanzar;
		setColor(PALETA[9]); std::cout << " ^ ";
		setColor(0); std::cout << " " << juego.arrayJugadores[i].mano.giroIzq;
		setColor(PALETA[9]);  std::cout << " < ";
		setColor(0); std::cout << " " << juego.arrayJugadores[i].mano.giroDer;
		setColor(PALETA[9]); std::cout << " > ";
		setColor(0); std::cout << " " << juego.arrayJugadores[i].mano.laser;
		setColor(PALETA[9]);  std::cout << " ~ ";
		setColor(0);
		std::cout << std::endl;

	}

}


void reglas() {
	std::string linea;
	std::ifstream reglas;

	system("cls");
	reglas.open("reglas.txt");
	if(reglas.is_open()) {
		while (!reglas.eof())
		{
			std::getline(reglas, linea);
			std::cout << linea << std::endl;
		}
		reglas.close();
	}

	system("pause");
}
