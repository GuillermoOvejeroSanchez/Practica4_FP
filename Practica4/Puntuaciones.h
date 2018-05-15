#ifndef PUNTUACIONES_H
#define PUNTUACIONES_H

#include <string>

//using namespace std;

const int TAM_INICIAL = 4;

typedef struct{
	std::string nombre;
	int puntuacion;
}tInfoJugador;

typedef struct{
	tInfoJugador* ranking;	
	int tam;
	int contador;
}tPuntuaciones;

bool cargarPuntuaciones(tPuntuaciones &puntos); //Carga las puntuaciones de un archivo de texto a un array
void guardarPuntuaciones(const tPuntuaciones &puntos);  //Guarda las puntuaciones (del array a un txt) al salir del juego
void mostrarPuntuaciones(const tPuntuaciones &puntos); //Muestra las puntuaciones por pantalla
bool actualizarPuntuaciones(tPuntuaciones & puntos,const std::string & nombre, int nuevosPuntos); //Actualiza las puntuaciones nuevas y ordena el ranking
void redimensionar(tPuntuaciones & clasifiacion); //Aumenta en x2 el tama�o del array
void liberar(tPuntuaciones & clasificacion); //Libera la memoria dinamica de la clasificacion
void ordenAlfabetico(const tPuntuaciones & puntuaciones); //Muestra las puntuaciones por orden alfabetico
void ordenarPredeterminado(tPuntuaciones& puntuaciones);
#endif