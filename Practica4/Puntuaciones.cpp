#include "checkML.h"
#include "Puntuaciones.h"
#include <fstream>
#include <iostream>

using namespace std;

void ordenacionInserccion(tPuntuaciones& puntos, string nombre, int nuevosPuntos, int posicion); //Ordena el ranking por algoritmo de ordenacion

bool cargarPuntuaciones(tPuntuaciones& puntos) {

	ifstream archivo;
	tInfoJugador jugador;
	bool abierto = false;

	archivo.open("puntuaciones.txt");

	if (archivo.is_open()) {
		abierto = true;
		puntos.contador = 0;
		puntos.tam = TAM_INICIAL;
		puntos.ranking = new tInfoJugador[puntos.tam];
		
		archivo >> puntos.ranking[puntos.contador].nombre;
		archivo >> puntos.ranking[puntos.contador].puntuacion;
		puntos.contador++;

		while (!archivo.eof()) { //Incluye el nombre y la puntuacion en el array

			if (puntos.tam <= puntos.contador) //Si no hay hueco lo hacemos
				redimensionar(puntos);

			archivo >> puntos.ranking[puntos.contador].nombre;
			archivo >> puntos.ranking[puntos.contador].puntuacion;
			puntos.contador++;
		}
		puntos.contador--; //Decremento porque al mostrar las puntuaciones me muestra memoria vacia
	}
	archivo.close();
	return abierto;
}

void guardarPuntuaciones(const tPuntuaciones& puntos) {
	ofstream archivo;
	archivo.open("puntuaciones.txt");

	for (int i = 0; i < puntos.contador; i++)
		archivo << puntos.ranking[i].nombre << " " << puntos.ranking[i].puntuacion << endl;
	 
	archivo.close();

}

void mostrarPuntuaciones(const tPuntuaciones &puntos) {
	cout << endl;
	for (int i = 0; i < puntos.contador; i++)
		cout << i + 1 << ". " << puntos.ranking[i].nombre << " " << puntos.ranking[i].puntuacion << endl;
	system("pause");
}

bool actualizarPuntuaciones(tPuntuaciones& puntos,const string& nombre, int nuevosPuntos) {
	int i = 0;
	bool actualizado = false;
	bool nombreEncontrado = false;

	while (!nombreEncontrado && i < puntos.contador ) { //Busca a el jugador
		nombreEncontrado = (nombre == puntos.ranking[i].nombre);
		i++;
	}

	if (nombreEncontrado) { //Jugador ya esta en la lista
		puntos.ranking[i].puntuacion += nuevosPuntos;
		nuevosPuntos = puntos.ranking[i].puntuacion;

		ordenacionInserccion(puntos, nombre, nuevosPuntos, i);
		actualizado = true;
	}

	else { //No esta el jugador

		if (puntos.tam <= puntos.contador) //Si no hay hueco lo hacemos
			redimensionar(puntos);

		puntos.ranking[puntos.contador].nombre = nombre;
		puntos.ranking[puntos.contador].puntuacion = nuevosPuntos;

		ordenacionInserccion(puntos, nombre, nuevosPuntos, puntos.contador);
		puntos.contador++;
		actualizado = true;
	}


	return actualizado;
}

void ordenacionInserccion(tPuntuaciones& puntos, string nombre, int nuevosPuntos, int posicion) { //Algoritmo de ordenacion por insercion
	tInfoJugador aux;
	bool intercambio = false;
	while (posicion > 0 && intercambio) {

		intercambio = false;
		if (nuevosPuntos > puntos.ranking[posicion - 1].puntuacion) {

		//Intercambiar nuevo con posicion
		aux = puntos.ranking[posicion - 1];

		puntos.ranking[posicion - 1].nombre = nombre;
		puntos.ranking[posicion - 1].puntuacion = nuevosPuntos;

		puntos.ranking[posicion] = aux;
		posicion--;

		intercambio = true;
		}

	}
}


void redimensionar(tPuntuaciones& clasificacion) {
	tInfoJugador* old;
	old = clasificacion.ranking;
	clasificacion.tam *= 2;
	clasificacion.ranking = new tInfoJugador[clasificacion.tam];
	for (int i = 0; i < clasificacion.contador; i++)
	{
		clasificacion.ranking[i] = old[i];
	}
	delete[] old;

}

void liberar(tPuntuaciones & clasificacion) {

	delete[] clasificacion.ranking;
}


void ordenarPredeterminado(tPuntuaciones& puntuaciones) {

	bool intercambiar = true;
	tInfoJugador aux;

	int i, j;
	for (i = 0; i < puntuaciones.contador - 1; i++)
		// Last i elements are already in place   
		for (j = 0; j < puntuaciones.contador - i - 1; j++) {
			if (puntuaciones.ranking[j].nombre > puntuaciones.ranking[j + 1].nombre) {
				aux = puntuaciones.ranking[j];
				puntuaciones.ranking[j] = puntuaciones.ranking[j + 1];
				puntuaciones.ranking[j + 1] = aux;
				intercambiar = true;
			}
		}

	for (i = 0; i < puntuaciones.contador - 1; i++)
		// Last i elements are already in place   
		for (j = 0; j < puntuaciones.contador - i - 1; j++) {
			if (puntuaciones.ranking[j].puntuacion < puntuaciones.ranking[j + 1].puntuacion) {
				aux = puntuaciones.ranking[j];
				puntuaciones.ranking[j] = puntuaciones.ranking[j + 1];
				puntuaciones.ranking[j + 1] = aux;
				intercambiar = true;
			}
		}
}
void ordenAlfabetico(const tPuntuaciones & puntuaciones) {
	bool intercambiar = true;

	tInfoJugador* alfabetico;
	tInfoJugador aux;

	alfabetico = new tInfoJugador[puntuaciones.tam];

	for (int i = 0; i < puntuaciones.contador; i++)
	{
		alfabetico[i] = puntuaciones.ranking[i];
	}

	int i, j;
	for (i = 0; i < puntuaciones.contador - 1; i++)
		// Last i elements are already in place   
		for (j = 0; j < puntuaciones.contador - i - 1; j++) {
			if (alfabetico[j].nombre > alfabetico[j + 1].nombre) {
				aux = alfabetico[j];
				alfabetico[j] = alfabetico[j + 1];
				alfabetico[j + 1] = aux;
				intercambiar = true;
			}
		}


	for (int i = 0; i < puntuaciones.contador; i++)
	{
		cout << alfabetico[i].nombre << " " << alfabetico[i].puntuacion << endl;
	}
	delete[] alfabetico;
	system("pause");
}
