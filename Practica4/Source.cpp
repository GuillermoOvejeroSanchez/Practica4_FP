/*
GRUPO 1B DE FP
GUILLERMO OVEJERO SANCHEZ
MARLON JONATHAN CAMPOVERDE MENDEZ
*/

#include "checkML.h"
#include <iostream>
#include <ctime>
#include "Puntuaciones.h"
#include "Cartas.h"
#include "Dibujo.h"
#include "Juego.h"

bool menu(tJuego& juego, tPuntuaciones& puntuaciones); //Ejecuta las diferentes opciones del juego

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand((unsigned int)time(NULL));
	tPuntuaciones puntuaciones;
	tJuego juego;
	bool salir = false;

	if (cargarPuntuaciones(puntuaciones)) {
		while (salir != true) {

			salir = menu(juego, puntuaciones);
		}
	}
	return 0;
}


bool menu(tJuego& juego, tPuntuaciones& puntuaciones) {
	char opcion;
	bool salir = false;
	std::cout << "1).Jugar\n2).Mostrar puntuaciones\n3).Ver reglas e instrucciones\n\n0).Salir\n";
	std::cin >> opcion;
	std::cin.ignore(256, '\n');

	switch (opcion) {
	case '0': //Salir
		guardarPuntuaciones(puntuaciones); 
		liberar(puntuaciones);
		salir = true;
		break;
	case '1':  //Juego
		if (cargarJuego(juego)) {
			ejecutarJuego(juego, puntuaciones);
			ordenarPredeterminado(puntuaciones);
			for (int i = 0; i < juego.numJugadores; i++) //Puedo acceder a juego.jugadores?, o llamo desde otro lado
				liberar(juego.arrayJugadores[i].mazo);
		}
		else {
			std::cout << "No se ha podido cargar el tablero escogido" << std::endl;
			system("pause");
		}
		break;

	case '2': //Mostrar Puntuaciones
		std::cout << "1).Orden por defecto\n2).Orden Alfabetico\n";
		std::cin >> opcion;
		std::cin.ignore(256, '\n');
		std::cout << std::endl;

		switch (opcion) { //Submenu de puntuaciones
		case '1': mostrarPuntuaciones(puntuaciones); break;
		case '2': ordenAlfabetico(puntuaciones); break;
		}
		break;

	case '3': reglas(); break; //Ver reglas
	}

	system("cls");

	return salir;
}