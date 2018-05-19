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
bool actualizarPuntuaciones(tPuntuaciones & puntos,const std::string & nombre, int nuevosPuntos); //Actualiza las puntuaciones nuevas y ordena el ranking
void redimensionar(tPuntuaciones & clasifiacion); //Aumenta en x2 la capacidad del array
void liberar(tPuntuaciones & clasificacion); //Libera la memoria dinamica de la clasificacion y del ranking alfabetico
void ordenarPredeterminado(tPuntuaciones& puntuaciones); //Ordena segun su puntuacion principalmente y en caso de igualdad en orden alfabetico
void mostrarPuntuaciones(const tPuntuaciones &puntos); //Muestra las puntuaciones por pantalla
void ordenAlfabetico(const tPuntuaciones& puntuaciones); //Ordena las puntuaciones por orden alfabetico
#endif