#include "checkML.h"
#include "Juego.h"
#include "Dibujo.h"
#include <fstream>
#include <iostream>
#include <conio.h>

bool cargarTablero(tJuego& juego, std::string tablero);
char teclaToChar(int accion, tTecla& tecla, tCarta& carta);
tCasillas charToCasilla(char caracter);
void iniciarMano(tMano& mano);
bool usarCarta(tJuego& juego, tCarta carta);
bool manoVacia(const tMano& mano);
bool mazoVacio(const tMazo& mazo);
bool accion(tJuego& juego, tTecla tecla);
bool avanza(tJuego& juego, int numJugador, tDir direccion);
void giro(tJuego& juego, bool derecha = true);
bool laser(tJuego& juego);

bool cargarJuego(tJuego& juego) {
	tCarta carta;
	bool cargado = false;
	juego.fin = false;
	char jugadores;
	std::string _tablero;

	system("cls");

	std::cout << "Introduce nombre del tablero sin extension .txt: ";
	//std::cin >> _tablero;
	//_tablero += ".txt";
	_tablero = "tableros.txt"; //Debug

	std::cout << "Numero de jugadores: ";
	std::cin >> jugadores;

	while (jugadores < '1' || jugadores > '4' )
	{
		std::cin.ignore(256, '\n');
		std::cin >> jugadores;
	}
	juego.numJugadores = int(jugadores - '0');
	juego.jugadores = juego.numJugadores;

	for (int i = 0; i < juego.numJugadores; i++) { //Introduce nombres de jugadores
		std::cin.ignore(256, '\n');
		std::cout << "Jugador " << i + 1 << " introduce tu nombre: ";
		std::cin >> juego.arrayJugadores[i].nombre;
	}

	if (cargarTablero(juego, _tablero)) {

		for (int i = 0; i < juego.numJugadores; i++) { //Genera las manos y los mazos de los jugadores
			crearVacia(juego.arrayJugadores[i].mazo);
			crearMazoAleatorio(juego.arrayJugadores[i].mazo);
			iniciarMano(juego.arrayJugadores[i].mano);
			juego.arrayJugadores[i].estaJugando = true;
			juego.arrayJugadores[i].coordInicial = juego.arrayJugadores[i].coordenada;


			for (int j = 0; j < 3; j++) { //Roba 3 cartas iniciales
				sacar(juego.arrayJugadores[i].mazo, carta);
				incluirCarta(juego.arrayJugadores[i].mano, carta);
			}
			cargado = true;
		}
	}
	return cargado;
}

bool cargarTablero(tJuego& juego, std::string tablero) {
	int numJugador = 0;
	int numHielo = 0;
	bool cargado = false;
	char caracter = NULL;
	std::string basura;
	std::ifstream archivo;
	tEstadoCasilla casilla;

	archivo.open(tablero);

	if (archivo.is_open()) {
		while (!archivo.eof() && caracter != char(juego.numJugadores + int('0')))
			archivo >> caracter;

		getline(archivo, basura);

		for (int i = 0; i < NUM_FILAS; i++) {
			for (int j = 0; j < NUM_COLUMNAS; j++) {
				archivo.get(caracter);
				casilla = charToCasilla(caracter).estado;
				juego.tablero[i][j].estado = casilla;

				if (casilla == TORTUGA) { //Asignacion Numero y Dir de Tortuga
					int jugador = 0;
					juego.tablero[i][j].tortuga.numero = numJugador;
					juego.arrayJugadores[numJugador].coordenada.fila = i;
					juego.arrayJugadores[numJugador].coordenada.columna = j;
					juego.tablero[i][j].tortuga.direccion = charToCasilla(caracter).tortuga.direccion;
					numJugador++;
				}

			}
			getline(archivo, basura); //Evita coger los saltos de linea del archivo
		}
		cargado = true;
		archivo.close();
	}
	return cargado;
}

void mostrarJuego(const tJuego& juego) { //Funciones de "Dibujo"
	mostrarTablero(juego.tablero);
	mostrarJugadores(juego);
}

bool ejecutarTurno(tJuego& juego,bool& finJugador) {
	bool ejecutado = false;
	bool fin = false;
	char accion;
	tMano& mano = juego.arrayJugadores[juego.turnoActual].mano;
	tMazo& mazo = juego.arrayJugadores[juego.turnoActual].mazo;
	std::string& nombre = juego.arrayJugadores[juego.turnoActual].nombre;

	std::cout << "Robar R, Introducir Secuencia E: ";

	do {
		std::cin >> accion; //Solicita una accion Robar o Introducir Secuencia
		//std::cin.ignore(256, '\n'); //Limpia el buffer para evitar robar multiples veces
	}
	while ((accion != 'R' && accion != 'r') && (accion != 'E' && accion != 'e'));
		

	if (accion == 'R' || accion == 'r' && !mazoVacio(mazo)) // Accion Robar
			ejecutado = accionRobar(juego);

	else {	//Accion Introducir Secuencia

		if (manoVacia(mano)) { //Si la mano esta vacia te obliga a robar una carta
			std::cout << "No tienes cartas, roba una." << std::endl;
			system("pause");
			ejecutado = accionRobar(juego);
		}

		else { //Ejecuta una secuencia de cartas

			if (mazoVacio(mazo)) //Si el mazo esta vacio te obliga a jugar una secuencia
				std::cout << "Mazo vacio, ejecuta una secuencia: ";

			fin = accionSecuencia(juego, mazo);
			if (fin) { //Fin de juego
				finJugador = true;
				esFinDePartida(juego);
			}
			ejecutado = true;
		}
	}

	return ejecutado;
}

bool accionRobar(tJuego& juego) {
	bool robado = false;
	tCarta carta;
	tMazo& mazo = juego.arrayJugadores[juego.turnoActual].mazo;
	tMano& mano = juego.arrayJugadores[juego.turnoActual].mano;

	if (sacar(mazo, carta)) { //Saca la carta de arriba del mazo
	incluirCarta(mano, carta); //Incluye una carta en tu mano
	robado = true;
	}
	return robado;
}

bool accionSecuencia(tJuego& juego, tMazo& cartas) {
	tTecla tecla;
	tCarta carta;
	std::string basura;
	char caracter;
	bool finaliza = false;
	int _accion;
	int enter = 13;

	_accion = _getch();
	while (_accion != enter) //Enter para salir
	{
		//ACCIONES
		if (_accion == 0xe0) { //Avanzar y direcciones
			_accion = _getch();

			if (_accion == 80)
				std::cout << "\n[ADVERTENCIA]: No puedes ir hacia atras\n";
			else {
				caracter = teclaToChar(_accion, tecla, carta);
				if (usarCarta(juego, carta)) {
					finaliza = accion(juego, tecla);
					mostrarJuego(juego); //Ver accion por accion (Debug)
					insertar(juego.arrayJugadores[juego.turnoActual].mazo, carta);
					std::cout << caracter << " ";
				}
			}
		}

		else if (_accion == ' ') { //Laser
			tecla = _LASER;
			carta = LASER;
			if (usarCarta(juego, carta)) {
				finaliza = accion(juego, tecla);
				insertar(juego.arrayJugadores[juego.turnoActual].mazo, carta);
				std::cout << "~ ";
			}
		}

		else //Cualquier otra tecla es incorrecta
			std::cout << "\n[ERROR]: Tecla Incorrecta\n";


		if (!finaliza)
			_accion = _getch();
		else
			_accion = enter;
	}
	
	return finaliza;
}

void cambiarTurno(tJuego& juego) {
	juego.turnoActual++;
	juego.turnoActual = juego.turnoActual % juego.numJugadores;
	if (juego.arrayJugadores[juego.turnoActual].estaJugando == false && juego.jugadores > 0)
		cambiarTurno(juego);

}

bool esFinDePartida(tJuego& juego) {
	juego.fin = false;
	char opcion;
	juego.jugadores--;

		if (juego.jugadores <= 0)
			juego.fin = true;

		else {
			std::cout << "\nHAS LLEGADO A LA JOYA!\nQuieres seguir jugando? Y/N\n";
			std::cin >> opcion;
			while ((opcion != 'Y' && opcion != 'y') && (opcion != 'N' && opcion != 'n'))
				std::cin >> opcion;

			if (opcion == 'N' || opcion == 'n') {
				juego.fin = true;
				
			}
		}
	return juego.fin;
}

void incluirCarta(tMano& mano, tCarta carta) {
		switch (carta) {
		case AVANZAR: mano.avanzar++;   break;
		case GIRODERECHA: mano.giroDer++ ; break;
		case GIROIZQUIERDA: mano.giroIzq++ ; break;
		case LASER: mano.laser++; break;
		}

}

tCasillas charToCasilla(char caracter) {

	tCasillas casilla;
	switch (caracter)
	{
	case ' ': casilla.estado = VACIA; break;
	case '#': casilla.estado = MURO; break;
	case '@': casilla.estado = HIELO; break;
	case 'C': casilla.estado = CAJA; break;
	case '$': casilla.estado = JOYA; break;
	case 'R': casilla.estado = TORTUGA; casilla.tortuga.direccion = ESTE; break;
	case 'D': casilla.estado = TORTUGA; casilla.tortuga.direccion = SUR; break;
	case 'U': casilla.estado = TORTUGA; casilla.tortuga.direccion = NORTE; break;
	case 'L': casilla.estado = TORTUGA; casilla.tortuga.direccion = OESTE; break;
	default: casilla.estado = NUM_TIPOS_CASILLAS; break;
	}
	return casilla;

}

void iniciarMano(tMano& mano) {
	mano.avanzar = 0;
	mano.giroDer = 0;
	mano.giroIzq = 0;
	mano.laser = 0;
}

char teclaToChar(int accion, tTecla& tecla, tCarta& carta) {
	char caracter = NULL;
	switch (accion)
	{
	case 75: caracter = '<'; tecla = _IZQUIERDA; carta = GIROIZQUIERDA; break;
	case 77: caracter = '>'; tecla = _DERECHA;  carta = GIRODERECHA;  break;
	case 72: caracter = '^'; tecla = _AVANZA; carta = AVANZAR; break;
	}
	return caracter;

}

bool usarCarta(tJuego& juego, tCarta carta) {
	bool usada = false;
	tMano& mano = juego.arrayJugadores[juego.turnoActual].mano;

	switch (carta) {
	case AVANZAR: if (mano.avanzar > 0) {
		mano.avanzar--;
		usada = true;
	}
		break;
	case GIRODERECHA:if (mano.giroDer > 0) {
		mano.giroDer--;
		usada = true;
	}
		break;
	case GIROIZQUIERDA: if (mano.giroIzq > 0) {
		mano.giroIzq--;
		usada = true;
	}
		break;
	case LASER: if (mano.laser > 0) {
		mano.laser--;
		usada = true;
	}
		break;
	}

	return usada;

}

bool manoVacia(const tMano& mano) {
	bool vacio = false;
	if (mano.avanzar == 0 && mano.giroDer == 0 && mano.giroIzq == 0 && mano.laser == 0)
		vacio = true;
	return vacio;

}

bool mazoVacio(const tMazo& mazo) {
	bool vacio = false;
	if (mazo.numCartas == 0)
		vacio = true;

	return vacio;
}

bool accion(tJuego& juego, tTecla tecla) {
	bool finaliza = false;

	tCoordenada coordenada = juego.arrayJugadores[juego.turnoActual].coordenada;
	tDir direccion = juego.tablero[coordenada.fila][coordenada.columna].tortuga.direccion;

	switch (tecla) {
	case _AVANZA: finaliza = avanza(juego, juego.turnoActual, direccion ); 
		break;
	case _DERECHA: giro(juego);
		break;
	case _IZQUIERDA: giro(juego, false );
		break;
	case _LASER: finaliza = laser(juego);
		break;
	}

	return finaliza;

}

bool avanza(tJuego& juego, int numJugador, tDir direccion) { 
	


	//Meter referencia de punteros
	int& _fila = juego.arrayJugadores[numJugador].coordenada.fila;
	int& _columna = juego.arrayJugadores[numJugador].coordenada.columna;
	int filaIni = _fila;
	int columnaIni = _columna;
	tTortuga& _tortuga = juego.tablero[_fila][_columna].tortuga;
	tCoordenada ultPos;
	
	ultPos.fila = filaIni;
	ultPos.columna = columnaIni;


	switch (direccion) { //Mueve una casilla en la direccion indicada
	case NORTE:  _fila--; break;
	case OESTE: _columna--; break;
	case SUR: _fila++;  break;
	case ESTE: _columna++; break;
	}

	tEstadoCasilla& estado = juego.tablero[_fila][_columna].estado;

	if (estado == HIELO || estado == MURO || estado == TORTUGA || _fila >= NUM_FILAS || _columna >= NUM_COLUMNAS || _fila < 0 || _columna < 0) { //Avanzar contra un obstaculo
		_fila = filaIni;
		_columna = columnaIni;
	}

	else if (estado == CAJA) { //Avanzar contra una caja
		int filaCaja = _fila;
		int columnaCaja = _columna;
		int filaSig = _fila; //Fila siguiente a la caja
		int columnaSig = _columna; //Columna siguiente a la caja

		switch (direccion) { //Mueve la caja e indica la posicion siguiente a la caja
		case NORTE:  filaCaja--; filaSig--; break;
		case OESTE: columnaCaja--;  columnaSig--; break;
		case SUR: filaCaja++;  filaSig++;  break;
		case ESTE: columnaCaja++;  columnaSig++; break;
		}

		tEstadoCasilla& estadoCaja = juego.tablero[filaSig][columnaSig].estado;

		if (estadoCaja == HIELO || estadoCaja == MURO || estadoCaja == TORTUGA || estadoCaja == CAJA || estadoCaja == JOYA || filaSig > NUM_FILAS || columnaSig > NUM_COLUMNAS || filaSig < 0 || columnaSig < 0) {
			juego.tablero[_fila][_columna].estado = CAJA;
			_fila = filaIni;
			_columna = columnaIni;
		}
		else //Si no hay ningun obstaculo mueve la caja
			juego.tablero[filaCaja][columnaCaja].estado = CAJA;
	}

	else if (juego.tablero[_fila][_columna].estado == JOYA){ //Llegar a la joya
		juego.fin = true;
		juego.tablero[_fila][_columna].estado = VACIA; //Vacia la casilla de la joya
		juego.arrayJugadores[numJugador].estaJugando = false; //Elimina al jugador
	}

	//Establece datos del jugador
	juego.tablero[filaIni][columnaIni].estado = VACIA;
	if (!juego.fin) { //juego.arrayJugadores[juego.turnoActual].estaJugando == true
		juego.tablero[_fila][_columna].estado = TORTUGA;
		juego.tablero[_fila][_columna].tortuga.direccion = direccion;
		juego.tablero[_fila][_columna].tortuga.numero = numJugador;
	}

	return juego.fin;
}

void giro(tJuego& juego, bool derecha) {
	int& _fila = juego.arrayJugadores[juego.turnoActual].coordenada.fila;
	int& _columna = juego.arrayJugadores[juego.turnoActual].coordenada.columna;
	tDir&  direccion= juego.tablero[_fila][_columna].tortuga.direccion;
	int giro = 1; //Derecha
	if (derecha == false)
	{
		giro = 3; //Izquierda
	}
	direccion = tDir(int(direccion + giro ) % 4);
}

bool laser(tJuego& juego) {
	bool finaliza = false;
	int _fila = juego.arrayJugadores[juego.turnoActual].coordenada.fila;
	int _columna = juego.arrayJugadores[juego.turnoActual].coordenada.columna;
	int filaIni = juego.arrayJugadores[juego.turnoActual].coordenada.fila;
	int columnaIni= juego.arrayJugadores[juego.turnoActual].coordenada.columna;
	tDir direccion = juego.tablero[_fila][_columna].tortuga.direccion;
	tCoordenada inicial = juego.arrayJugadores[juego.turnoActual].coordInicial;

	do
	{
		switch (direccion) { //Mueve una casilla en la direccion indicada
		case NORTE:  _fila--; break;
		case OESTE: _columna--; break;
		case SUR: _fila++;  break;
		case ESTE: _columna++; break;
		}
	} while (juego.tablero[_fila][_columna].estado == VACIA );

	if(juego.tablero[_fila][_columna].estado == HIELO) //LASER ~ HIELO
		juego.tablero[_fila][_columna].estado = VACIA;

	else if (juego.tablero[_fila][_columna].estado == JOYA) { //LASER ~ JOYA
		if (juego.tablero[inicial.fila][inicial.columna].estado != TORTUGA && juego.tablero[inicial.fila][inicial.columna].estado != CAJA) {
		juego.tablero[filaIni][columnaIni].estado = VACIA;
		juego.arrayJugadores[juego.turnoActual].coordenada = inicial;
		juego.tablero[inicial.fila][inicial.columna].estado = TORTUGA;
		juego.tablero[inicial.fila][inicial.columna].tortuga.direccion = direccion;
		juego.tablero[inicial.fila][inicial.columna].tortuga.numero = juego.turnoActual;
		}
	}

	else if (juego.tablero[_fila][_columna].estado == TORTUGA) { //LASER ~ TORTUGA
		
		int numjugador = juego.tablero[_fila][_columna].tortuga.numero;

		tEstadoCasilla& estado = juego.tablero[_fila][_columna].estado;
		
		juego.tablero[_fila][_columna].estado = VACIA;
		do
		{
			switch (direccion) { //Mueve una casilla en la direccion indicada
			case NORTE:  _fila--; break;
			case OESTE: _columna--; break;
			case SUR: _fila++;  break;
			case ESTE: _columna++; break;
			}
			finaliza = avanza(juego, numjugador, direccion);
		} while (!(estado == HIELO || estado == MURO || estado == TORTUGA || _fila >= NUM_FILAS || _columna >= NUM_COLUMNAS || _fila < 0 || _columna < 0));
		
	}

	
	return finaliza;

}

void ejecutarJuego(tJuego& juego,tPuntuaciones& puntuaciones ) {
	int puesto = 1;
	bool finaliza = false;
	while (!juego.fin) {
		finaliza = false;
		mostrarJuego(juego);
		if (ejecutarTurno(juego, finaliza)) {
			if (finaliza == true || juego.fin == true) { //Finaliza un jugador o el juego
				mostrarJuego(juego);
				std::cout << std::endl << std::endl << "HAS GANADO!" << std::endl << std::endl;
				system("pause");
				actualizarPuntuaciones(puntuaciones, juego.arrayJugadores[juego.turnoActual].nombre, (juego.numJugadores - puesto + 1));
				puesto++;
			}
			cambiarTurno(juego);
			system("cls");
		}
	}
}

