#ifndef CARTAS_H
#define CARTAS_H

int const MAX_AVANZAR = 18;
int const MAX_LADO = 8;
int const MAX_LASER = 4;
int const MAX_CARTAS = MAX_AVANZAR + MAX_LADO*2 + MAX_LASER ;

typedef enum { AVANZAR, GIROIZQUIERDA, GIRODERECHA, LASER } tCarta;


typedef tCarta* tArrayPtrCartas[MAX_CARTAS];

typedef struct {
	int inicio;
	int numCartas;
	tArrayPtrCartas cartas;
} tMazo;

void crearVacia(tMazo & mazo); //Inicializa el mazo a 0
//false --> abajo true --> arriba
bool sacar(tMazo & mazo, tCarta& carta); //Introduce una carta en tu mano (arriba o abajo)
void insertar(tMazo & mazo, tCarta carta); //Introduce una carta en el mazo (arriba o abajo)
void crearMazoAleatorio(tMazo & mazo); //Genera un mazo con todas las cartas y luego lo barajea
void liberar(tMazo & mazo); //libera la memoria dinamica creada para el mazo.
 //
#endif