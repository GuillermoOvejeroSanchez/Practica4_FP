#ifndef JUEGO_H
#define JUEGO_H

#include "Puntuaciones.h"
#include "Cartas.h"
#include <string>

const int MAX_TORTUGAS = 4;
const int MAX_HIELO = 12;
const int NUM_FILAS = 8;
const int NUM_COLUMNAS = 8;
const int PALETA [6 + MAX_TORTUGAS] = {1,11,7,4,12,2,13,9,10,3}; //VACIA , HIELO, MURO, CAJA, JOYA, TORTUGA1, TORTUGA2, TORTUGA3, TORTUGA4, CARTAS 

typedef enum {NORTE, ESTE, SUR, OESTE} tDir;

typedef enum {_AVANZA, _DERECHA, _IZQUIERDA, _LASER, _SALIR} tTecla;

typedef enum {VACIA, HIELO, MURO, CAJA, JOYA, TORTUGA, NUM_TIPOS_CASILLAS} tEstadoCasilla;

typedef enum{SECUENCIA, ROBAR} tAccion;

typedef struct {
	int numero;
	tDir direccion;
} tTortuga;

typedef struct {
	tEstadoCasilla estado;
	tTortuga tortuga;
} tCasillas ;

typedef tCasillas tTablero[NUM_FILAS][NUM_COLUMNAS];

typedef struct {
	int fila;
	int columna;
} tCoordenada;

typedef struct {
	int avanzar = 0;
	int giroIzq = 0;
	int giroDer = 0;
	int laser = 0;
} tMano;

typedef struct {
	tMazo mazo;
	tMano mano;
	tCoordenada coordenada;
	std::string nombre;
	tCoordenada coordInicial;
	bool estaJugando;
} tJugador;

typedef struct {
	int numJugadores;
	int jugadores;
	int turnoActual = 0;
	tJugador arrayJugadores[MAX_TORTUGAS];
	tTablero tablero;
	bool fin = false;
} tJuego;


bool cargarJuego(tJuego& juego); //Carga el tablero y los jugadores con sus mazos
void mostrarJuego(const tJuego& juego); //Muestra el tablero y jugadores (Modulo Dibujo)
bool ejecutarTurno(tJuego& juego, bool& finJug); //Ejecuta el turno de un jugador
bool accionRobar(tJuego& juego); //Roba una carta si se puede
bool accionSecuencia(tJuego& juego, tMazo& cartas); //Ejecuta una secuencia de cartas
void cambiarTurno(tJuego& juego); //Cambia el turno del jugador
bool esFinDePartida(tJuego& juego); //Cuando un jugador gana te permite seguir jugando o no
void incluirCarta(tMano& mano, tCarta carta); //Incluye una carta en la mano del jugador
void ejecutarJuego(tJuego& juego, tPuntuaciones& puntuaciones); //Ejecuta una partida completa del juego


#endif
