#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
using namespace std;

list<Action> AvanzaASaltosDeCaballo(){		//Moviemiento en L de caballo

	list<Action> secuencia;
	secuencia.push_back(act_CLB_TURN_SR);
	secuencia.push_back(act_CLB_WALK);

	return secuencia;
}
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	

	if(sensores.nivel != 4){
		if(!hayPlan){

			//Invocar aquí al método de busqueda
			cout << "Calculamos un nuevo plan " << endl;

			//NIVEL 0
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;

			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;

			//NIVEL 1
			col_state.jugador.f = sensores.posF;
			col_state.jugador.c = sensores.posC;
			col_state.jugador.brujula = sensores.sentido;

			col_state.colaborador.f = sensores.CLBposF;
			col_state.colaborador.c = sensores.CLBposC;
			col_state.colaborador.brujula = sensores.CLBsentido;

			//NIVEL 2
			c_state2.jugador.f = sensores.posF;
			c_state2.jugador.c = sensores.posC;
			c_state2.jugador.brujula = sensores.sentido;

			c_state2.colaborador.f = sensores.CLBposF;
			c_state2.colaborador.c = sensores.CLBposC;
			c_state2.colaborador.brujula = sensores.CLBsentido;

			//NIVEL 3
			c_state3.jugador.f = sensores.posF;
			c_state3.jugador.c = sensores.posC;
			c_state3.jugador.brujula = sensores.sentido;

			c_state3.colaborador.f = sensores.CLBposF;
			c_state3.colaborador.c = sensores.CLBposC;
			c_state3.colaborador.brujula = sensores.CLBsentido;

			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			if(primero){	//Para inicializaciones
				//Nivel1
				col_state.ultimaOrdenColaborador = act_CLB_STOP;
				//Nivel2
				c_state2.bikini = false;
				c_state2.zapatillas = false;
				c_state2.coste = 0;
				//Nivel3
				c_state3.ultimaOrdenColaborador = act_CLB_STOP;
				c_state3.bikini = false;
				c_state3.zapatillas = false;
				c_state3.bikini_col = false;
				c_state3.zapatillas_col = false;
				c_state3.coste = 0;

				primero = false;
			}
			
			switch(sensores.nivel){
				case 0: 
					plan = AnchuraSoloJugador_V3(c_state,goal,mapaResultado);
				break;

				case 1: //Incluir aquí la llamada al algoritmo de búsqueda del nivel 1
					plan = AnchuraN1(col_state,goal,mapaResultado);	
				break;

				case 2: //Incluir aquí la llamada al algoritmo de búsqueda del nivel 2
					plan = DijkstraN2(c_state2,goal,mapaResultado);
				break;

				case 3: //Incluir aquí la llamada al algoritmo de búsqueda del nivel 3
					plan = AestrellaN3(c_state3,goal,mapaResultado);
				break;
			}

			if(plan.size() > 0){

				VisualizaPlan(c_state,plan);
				hayPlan = true;
				cout<< "Detecta un plan" << endl;
			}	
		}

		if(hayPlan && plan.size()>0){

			accion = plan.front();
			plan.pop_front();
		}

		if (plan.size() == 0){

			cout << "Se completó el plan" << endl;
			hayPlan = false;
		}
	}
	else{

		//Incluir aqui la solucion al nivel 4
	}

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
//Devuelve si una ubicacion en el mapa es transitable para el agente
bool ComportamientoJugador::CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa){

	return (mapa[x.f][x.c] != 'P' && mapa[x.f][x.c] != 'M');
}

//Devuelve la ubicacion siguiente según el avance del agente
ubicacion ComportamientoJugador::NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	switch(pos.brujula){

		case norte: 
			next.f = next.f - 1; 
		break;
		case noreste:
			next.f = next.f - 1;
			next.c = next.c + 1;
		break;
		case este:
			next.c = next.c + 1;
		break;
		case sureste:
			next.f = next.f + 1;
			next.c = next.c + 1;
		break;
		case sur:
			next.f = next.f + 1; 
		break;
		case suroeste:
			next.f = next.f + 1;
			next.c = next.c - 1;
		break;
		case oeste: 
			next.c = next.c - 1;
		break;
		case noroeste:
			next.f = next.f - 1;
			next.c = next.c - 1;
		break;
	}

	return next;
}

/*Devuelve el estado que se genera si el agente puede avanzar
Si no se puede avanzar, devuelve como salida el mismo estado de entrada*/
stateN0 ComportamientoJugador::apply(const Action & a, const stateN0 & st, const vector<vector<unsigned char>> &mapa){
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;

	switch(a){
		case actWALK:	//Si la prox casilla es transitable y no está ocupada por el colaborador
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

				st_result.jugador = sig_ubicacion;
			}
		break;

		case actRUN:	//Si la 2 prox casillas con transitables, y no estan ocupadas por el colaborador
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if(CasillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)){

					st_result.jugador = sig_ubicacion2;
				}
			}
		break;
		
		case actTURN_L:		//90ª a la izq
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

		case actTURN_SR:	//45º a la der
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;
	}

	return st_result;
}


stateN1 ComportamientoJugador::applyN1(const Action & a, const stateN1 & st, const vector<vector<unsigned char>> &mapa){
	stateN1 st_result = st;
	ubicacion sig_ubicacion,sig_ubicacion2;	//Para el jugador
	ubicacion sig_ubicacion_col;			//Para el colaborador

		
		if(a != act_CLB_WALK && a != act_CLB_STOP &&  a != act_CLB_TURN_SR){	//Acciones de concatenacion jugaodr colaborador
			switch(st.ultimaOrdenColaborador){
				case act_CLB_WALK:	//Si la prox casilla es transitable, no esta el jugador, y es visible por el jugador
					switch(a){
							case actWALK:	//Si la prox casilla es transitable
							sig_ubicacion_col = NextCasilla(st.colaborador);
							sig_ubicacion = NextCasilla(st.jugador);
							if(CasillaTransitable(sig_ubicacion_col, mapa) && CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c) 
							&& !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c) && !(sig_ubicacion.c == sig_ubicacion_col.c && sig_ubicacion.f == sig_ubicacion_col.f)){

								st_result.jugador = sig_ubicacion;
								st_result.colaborador = sig_ubicacion_col;
							}
						break;
						case actRUN:	//Si la 2 prox casillas con transitables
							
							sig_ubicacion_col = NextCasilla(st.colaborador);
							sig_ubicacion = NextCasilla(st.jugador);
							if(CasillaTransitable(sig_ubicacion_col, mapa) && CasillaTransitable(sig_ubicacion, mapa) && 
							!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c) && 
							!(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c) && !(sig_ubicacion.c == sig_ubicacion_col.c && sig_ubicacion.f == sig_ubicacion_col.f)){

								sig_ubicacion2 = NextCasilla(sig_ubicacion);
								if(CasillaTransitable(sig_ubicacion_col, mapa) && CasillaTransitable(sig_ubicacion2, mapa) && 
								!(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)
								&& !(sig_ubicacion2.c == sig_ubicacion_col.c && sig_ubicacion2.f == sig_ubicacion_col.f)){

									st_result.jugador = sig_ubicacion2;
									st_result.colaborador = sig_ubicacion_col;
								}
							}
						break;
						case actTURN_L:		//90ª a la izq
							sig_ubicacion_col = NextCasilla(st.colaborador);
							if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){

								st_result.colaborador = sig_ubicacion_col;
								st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
							}
							
						break;
						case actTURN_SR:	//45º a la der
							sig_ubicacion_col = NextCasilla(st.colaborador);
							if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){

								st_result.colaborador = sig_ubicacion_col;
								st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
							}
						break;
					}
				break;

				case act_CLB_STOP:	//Se queda quieto
						//st_result.ultimaOrdenColaborador = act_CLB_STOP;
						switch(a){
							case actWALK:	//Si la prox casilla es transitable
							sig_ubicacion = NextCasilla(st.jugador);
							if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

								st_result.jugador = sig_ubicacion;
							}
						break;
						case actRUN:	//Si la 2 prox casillas con transitables
							sig_ubicacion = NextCasilla(st.jugador);
							if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

								sig_ubicacion2 = NextCasilla(sig_ubicacion);
								if(CasillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)){

									st_result.jugador = sig_ubicacion2;
								}
							}
						break;
						case actTURN_L:		//90ª a la izq
							st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
						break;

						case actTURN_SR:	//45º a la der
							st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
						break;
					}
				break;
			
				case act_CLB_TURN_SR:	//45º a la der
					switch(a){
							case actWALK:	//Si la prox casilla es transitable
							sig_ubicacion = NextCasilla(st.jugador);
							if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

								st_result.jugador = sig_ubicacion;
								st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
							}
						break;
						case actRUN:	//Si la 2 prox casillas con transitables
							sig_ubicacion = NextCasilla(st.jugador);
							if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

								sig_ubicacion2 = NextCasilla(sig_ubicacion);
								if(CasillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)){

									st_result.jugador = sig_ubicacion2;
									st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
								}
							}
						break;
						case actTURN_L:		//90ª a la izq
							st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
							st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
						break;

						case actTURN_SR:	//45º a la der
							st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
							st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
						break;
					}
				break;
			}
		}
		else{	//Acciones del colaborador
			switch(a){
				case act_CLB_WALK:	//Si la prox casilla es transitable, no esta el jugador, y es visible por el jugador
					sig_ubicacion_col = NextCasilla(st.colaborador);
					if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){

						st_result.colaborador = sig_ubicacion_col;
						st_result.ultimaOrdenColaborador = act_CLB_WALK;
					}
				break;
				case act_CLB_STOP:	//Se queda quieto

					st_result.colaborador = st.colaborador;	//No se modifica nada, se queda quieto
					st_result.ultimaOrdenColaborador = act_CLB_STOP;
				break;
				case act_CLB_TURN_SR:	//45º a la der
					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
					st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
				break;
			}
		}
	

	return st_result;
}

stateN2 ComportamientoJugador::applyN2(const Action & a, const stateN2 & st, const vector<vector<unsigned char>> &mapa){
	stateN2 st_result = st;
	ubicacion sig_ubicacion,sig_ubicacion2;
	char posj = mapa[st.jugador.f][st.jugador.c];	//posicion del jugador antes del movimiento

	switch(a){
		case actWALK:	//Si la prox casilla es transitable
		sig_ubicacion = NextCasilla(st.jugador);
		if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

			if(posj == 'A'){	//Agua
				if(st.bikini){	//Compruebo si tengo bikini

					st_result.coste = st_result.coste + 10;
				}else{

					st_result.coste = st_result.coste + 100;
				}
			}
			else if(posj == 'B'){	//Bosque
				if(st.zapatillas){	//Compruebo si tengo zapatillas

					st_result.coste = st_result.coste + 15;
				}else{

					st_result.coste = st_result.coste + 50;
				}
			}
			else if(posj == 'T'){	//Tierra
				
				st_result.coste = st_result.coste + 2;	
			}
			else{					//Demás casillas

				st_result.coste = st_result.coste + 1;
			}

			//Revisamos si la casilla es de bikini o zapatillas
			char casilla = mapa[st_result.jugador.f][st_result.jugador.c];
			if(casilla == 'K'){

				st_result.bikini = true;
				st_result.zapatillas = false;
			}
			else if(casilla == 'D'){

				st_result.zapatillas = true;
				st_result.bikini = false;
			}
			st_result.jugador = sig_ubicacion;
		}
		break;
		case actRUN:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){
				
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if(CasillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)){
					if(posj == 'A'){	//Agua
						if(st.bikini){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 15;
						}else{

							st_result.coste = st_result.coste + 150;
						}
					}
					else if(posj == 'B'){	//Bosque
						if(st.zapatillas){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 25;
						}else{

							st_result.coste = st_result.coste + 75;
						}
					}
					else if(posj == 'T'){	//Tierra
						
						st_result.coste = st_result.coste + 3;	
					}
					else{					//Demás casillas

						st_result.coste = st_result.coste + 1;
					}

					//Revisamos si la casilla es de bikini o zapatillas
					char casilla = mapa[st_result.jugador.f][st_result.jugador.c];
					if(casilla == 'K'){

						st_result.bikini = true;
						st_result.zapatillas = false;
					}
					else if(casilla == 'D'){

						st_result.zapatillas = true;
						st_result.bikini = false;
					}
					st_result.jugador = sig_ubicacion2;
				}
			}
		break;
		case actTURN_L:		//90ª a la izq
			if(posj == 'A'){	//Agua
				if(st.bikini){	//Compruebo si tengo bikini

					st_result.coste = st_result.coste + 5;
				}else{

					st_result.coste = st_result.coste + 30;
				}
			}
			else if(posj == 'B'){	//Bosque
				if(st.zapatillas){	//Compruebo si tengo zapatillas

					st_result.coste = st_result.coste + 1;
				}else{

					st_result.coste = st_result.coste + 7;
				}
			}
			else if(posj == 'T'){	//Tierra
						
				st_result.coste = st_result.coste + 2;	
			}
			else{	//Demás casillas

				st_result.coste = st_result.coste + 1;
			}

			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;
		case actTURN_SR:	//45º a la der
			if(posj == 'A'){	//Agua
				if(st.bikini){	//Compruebo si tengo bikini

					st_result.coste = st_result.coste + 2;
				}else{

					st_result.coste = st_result.coste + 10;
				}
			}
			else if(posj == 'B'){	//Bosque
				if(st.zapatillas){	//Compruebo si tengo zapatillas

					st_result.coste = st_result.coste + 1;
				}else{

					st_result.coste = st_result.coste + 5;
				}
			}
			else if(posj == 'T'){	//Tierra
						
				st_result.coste = st_result.coste + 1;	
			}
			else{	//Demás casillas

				st_result.coste = st_result.coste + 1;
			}
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;

	}
	return st_result;
}

stateN3 ComportamientoJugador::applyN3(const Action & a, const stateN3 & st, const vector<vector<unsigned char>> &mapa){
	stateN3 st_result = st;
	ubicacion sig_ubicacion,sig_ubicacion2, sig_ubicacion_col;
	char posj = mapa[st.jugador.f][st.jugador.c];	//posicion del jugador antes del movimiento
	char posc = mapa[st.colaborador.f][st.colaborador.c];;	//Para revisar la pos del colaborador antes del movimiento


	if(a != act_CLB_WALK && a != act_CLB_STOP &&  a != act_CLB_TURN_SR){	//Acciones de concatenacion jugaodr colaborador
		switch(st.ultimaOrdenColaborador){
			case act_CLB_WALK:
				switch(a){
					case actWALK:	//Si la prox casilla es transitable
					sig_ubicacion_col = NextCasilla(st.colaborador);
					sig_ubicacion = NextCasilla(st.jugador);
					if(CasillaTransitable(sig_ubicacion_col, mapa) && CasillaTransitable(sig_ubicacion, mapa) && 
					!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c) /*&& !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)*/
					&& !(sig_ubicacion.c == sig_ubicacion_col.c && sig_ubicacion.f == sig_ubicacion_col.f)){
						//COSTES PARA EL COLABORADOR
						if(posc == 'A'){	//Agua
							if(st.bikini_col){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 10;
							}else{

								st_result.coste = st_result.coste + 100;
							}
						}
						else if(posc == 'B'){	//Bosque
							if(st.zapatillas_col){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 15;
							}else{

								st_result.coste = st_result.coste + 50;
							}
						}
						else if(posc == 'T'){	//Tierra
								
							st_result.coste = st_result.coste + 2;	
						}
						else{					//Demás casillas

							st_result.coste = st_result.coste + 1;
						}

						//COSTES PARA EL JUGADOR
						if(posj == 'A'){	//Agua
							if(st.bikini){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 10;
							}else{

								st_result.coste = st_result.coste + 100;
							}
						}
						else if(posj == 'B'){	//Bosque
							if(st.zapatillas){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 15;
							}else{

								st_result.coste = st_result.coste + 50;
							}
						}
						else if(posj == 'T'){	//Tierra
							
							st_result.coste = st_result.coste + 2;	
						}
						else{					//Demás casillas

							st_result.coste = st_result.coste + 1;
						}
						st_result.jugador = sig_ubicacion;
						st_result.colaborador = sig_ubicacion_col;

						//Revisamos si la casilla es de bikini o zapatillas
						char casilla = mapa[st.colaborador.f][st.colaborador.c];
						if(casilla == 'K'){

							st_result.bikini_col = true;
							st_result.zapatillas_col = false;
						}
						else if(casilla == 'D'){

							st_result.zapatillas_col = true;
							st_result.bikini_col = false;
						}
						

						//Revisamos si la casilla es de bikini o zapatillas
						casilla = mapa[st.jugador.f][st.jugador.c];
						if(casilla == 'K'){

							st_result.bikini = true;
							st_result.zapatillas = false;
						}
						else if(casilla == 'D'){

							st_result.zapatillas = true;
							st_result.bikini = false;
						}

					
						}
					break;
					case actRUN:
					sig_ubicacion_col = NextCasilla(st.colaborador);
					sig_ubicacion = NextCasilla(st.jugador);
					if(CasillaTransitable(sig_ubicacion_col, mapa) && CasillaTransitable(sig_ubicacion, mapa) && 
					!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c) /*&& !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)*/
					&& !(sig_ubicacion.c == sig_ubicacion_col.c && sig_ubicacion.f == sig_ubicacion_col.f)){

						sig_ubicacion2 = NextCasilla(sig_ubicacion);
						if(CasillaTransitable(sig_ubicacion_col, mapa) && CasillaTransitable(sig_ubicacion2, mapa) && 
						!(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c) /*&& !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)*/
						&& !(sig_ubicacion2.c == sig_ubicacion_col.c && sig_ubicacion2.f == sig_ubicacion_col.f)){
							//COSTES PARA EL COLABORADOR
							if(posc == 'A'){	//Agua
								if(st.bikini_col){	//Compruebo si tengo bikini

									st_result.coste = st_result.coste + 10;
								}else{

									st_result.coste = st_result.coste + 100;
								}
							}
							else if(posc == 'B'){	//Bosque
								if(st.zapatillas_col){	//Compruebo si tengo zapatillas

									st_result.coste = st_result.coste + 15;
								}else{

									st_result.coste = st_result.coste + 50;
								}
							}
							else if(posc == 'T'){	//Tierra
									
								st_result.coste = st_result.coste + 2;	
							}
							else{					//Demás casillas

								st_result.coste = st_result.coste + 1;
							}

							//COSTES PARA EL JUGADOR
							if(posj == 'A'){	//Agua
								if(st.bikini){	//Compruebo si tengo bikini

									st_result.coste = st_result.coste + 15;
								}else{

									st_result.coste = st_result.coste + 150;
								}
							}
							else if(posj == 'B'){	//Bosque
								if(st.zapatillas){	//Compruebo si tengo zapatillas

									st_result.coste = st_result.coste + 25;
								}else{

									st_result.coste = st_result.coste + 75;
								}
							}
							else if(posj == 'T'){	//Tierra
								
								st_result.coste = st_result.coste + 3;	
							}
							else{					//Demás casillas

								st_result.coste = st_result.coste + 1;
							}
							st_result.jugador = sig_ubicacion2;
							st_result.colaborador = sig_ubicacion_col;

							//Revisamos si la casilla es de bikini o zapatillas
							char casilla = mapa[st.colaborador.f][st.colaborador.c];
							if(casilla == 'K'){

								st_result.bikini_col = true;
								st_result.zapatillas_col = false;
							}
							else if(casilla == 'D'){

								st_result.zapatillas_col = true;
								st_result.bikini_col = false;
							}

							//Revisamos si la casilla es de bikini o zapatillas
							casilla = mapa[st.jugador.f][st.jugador.c];
							if(casilla == 'K'){

								st_result.bikini = true;
								st_result.zapatillas = false;
							}
							else if(casilla == 'D'){

								st_result.zapatillas = true;
								st_result.bikini = false;
							}
							
						}
					}
					break;
					case actIDLE:
					sig_ubicacion_col = NextCasilla(st.colaborador);
					if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){
						//COSTES PARA EL COLABORADOR
						if(posc == 'A'){	//Agua
							if(st.bikini_col){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 10;
							}else{

								st_result.coste = st_result.coste + 100;
							}
						}
						else if(posc == 'B'){	//Bosque
							if(st.zapatillas_col){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 15;
							}else{

								st_result.coste = st_result.coste + 50;
							}
						}
						else if(posc == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 2;	
						}
						else{					//Demás casillas

							st_result.coste = st_result.coste + 1;
						}
						st_result.colaborador = sig_ubicacion_col;

						//Revisamos si la casilla es de bikini o zapatillas
						char casilla = mapa[st.colaborador.f][st.colaborador.c];
						if(casilla == 'K'){

							st_result.bikini_col = true;
							st_result.zapatillas_col = false;
						}
						else if(casilla == 'D'){

							st_result.zapatillas_col = true;
							st_result.bikini_col = false;
						}
						
					}
					
					break;
					case actTURN_L:
					sig_ubicacion_col = NextCasilla(st.colaborador);
					if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){
						//COSTES PARA EL COLABORADOR
						if(posc == 'A'){	//Agua
							if(st.bikini_col){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 10;
							}else{

								st_result.coste = st_result.coste + 100;
							}
						}
						else if(posc == 'B'){	//Bosque
							if(st.zapatillas_col){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 15;
							}else{

								st_result.coste = st_result.coste + 50;
							}
						}
						else if(posc == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 2;	
						}
						else{					//Demás casillas

							st_result.coste = st_result.coste + 1;
						}

						//COSTES PARA EL JUGADOR
						if(posj == 'A'){	//Agua
							if(st.bikini){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 5;
							}else{

								st_result.coste = st_result.coste + 30;
							}
						}
						else if(posj == 'B'){	//Bosque
							if(st.zapatillas){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 1;
							}else{

								st_result.coste = st_result.coste + 7;
							}
						}
						else if(posj == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 2;	
						}
						else{	//Demás casillas

							st_result.coste = st_result.coste + 1;
						}
						st_result.colaborador = sig_ubicacion_col;
						st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);

						//Revisamos si la casilla es de bikini o zapatillas
						char casilla = mapa[st.colaborador.f][st.colaborador.c];
						if(casilla == 'K'){

							st_result.bikini_col = true;
							st_result.zapatillas_col = false;
						}
						else if(casilla == 'D'){

							st_result.zapatillas_col = true;
							st_result.bikini_col = false;
						}

							
					}
					break;
					case actTURN_SR:
					sig_ubicacion_col = NextCasilla(st.colaborador);
					if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){
						//COSTES PARA EL COLABORADOR
						if(posc == 'A'){	//Agua
							if(st.bikini_col){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 10;
							}else{

								st_result.coste = st_result.coste + 100;
							}
						}
						else if(posc == 'B'){	//Bosque
							if(st.zapatillas_col){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 15;
							}else{

								st_result.coste = st_result.coste + 50;
							}
						}
						else if(posc == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 2;	
						}
						else{					//Demás casillas

							st_result.coste = st_result.coste + 1;
						}

						//COSTES PARA EL JUGADOR
						if(posj == 'A'){	//Agua
							if(st.bikini){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 2;
							}else{

								st_result.coste = st_result.coste + 10;
							}
						}
						else if(posj == 'B'){	//Bosque
							if(st.zapatillas){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 1;
							}else{

								st_result.coste = st_result.coste + 5;
							}
						}
						else if(posj == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 1;	
						}
						else{	//Demás casillas

							st_result.coste = st_result.coste + 1;
						}
						st_result.colaborador = sig_ubicacion_col;
						st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);

						//Revisamos si la casilla es de bikini o zapatillas
						char casilla = mapa[st.colaborador.f][st.colaborador.c];
						if(casilla == 'K'){

							st_result.bikini_col = true;
							st_result.zapatillas_col = false;
						}
						else if(casilla == 'D'){

							st_result.zapatillas_col = true;
							st_result.bikini_col = false;
						}

						
					}
					break;
				}
			break;
			case act_CLB_STOP:
				switch(a){
					case actWALK:	//Si la prox casilla es transitable
					sig_ubicacion = NextCasilla(st.jugador);
					if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

						if(posj == 'A'){	//Agua
							if(st.bikini){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 10;
							}else{

								st_result.coste = st_result.coste + 100;
							}
						}
						else if(posj == 'B'){	//Bosque
							if(st.zapatillas){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 15;
							}else{

								st_result.coste = st_result.coste + 50;
							}
						}
						else if(posj == 'T'){	//Tierra
							
							st_result.coste = st_result.coste + 2;	
						}
						else{					//Demás casillas

							st_result.coste = st_result.coste + 1;
						}
						st_result.jugador = sig_ubicacion;

						//Revisamos si la casilla es de bikini o zapatillas
						char casilla = mapa[st.jugador.f][st.jugador.c];
						if(casilla == 'K'){

							st_result.bikini = true;
							st_result.zapatillas = false;
						}
						else if(casilla == 'D'){

							st_result.zapatillas = true;
							st_result.bikini = false;
						}
						
					}
					break;
					case actRUN:
						sig_ubicacion = NextCasilla(st.jugador);
						if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){
							
							sig_ubicacion2 = NextCasilla(sig_ubicacion);
							if(CasillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)){
								if(posj == 'A'){	//Agua
									if(st.bikini){	//Compruebo si tengo bikini

										st_result.coste = st_result.coste + 15;
									}else{

										st_result.coste = st_result.coste + 150;
									}
								}
								else if(posj == 'B'){	//Bosque
									if(st.zapatillas){	//Compruebo si tengo zapatillas

										st_result.coste = st_result.coste + 25;
									}else{

										st_result.coste = st_result.coste + 75;
									}
								}
								else if(posj == 'T'){	//Tierra
									
									st_result.coste = st_result.coste + 3;	
								}
								else{					//Demás casillas

									st_result.coste = st_result.coste + 1;
								}
								st_result.jugador = sig_ubicacion2;
								//Revisamos si la casilla es de bikini o zapatillas
								char casilla = mapa[st.jugador.f][st.jugador.c];
								if(casilla == 'K'){

									st_result.bikini = true;
									st_result.zapatillas = false;
								}
								else if(casilla == 'D'){

									st_result.zapatillas = true;
									st_result.bikini = false;
								}
							}
						}
					break;
					case actIDLE:
					//El Colaborador stopea y el jugador tb		
					break;
					case actTURN_L:		//90ª a la izq
					//COSTES DEL JUGADOR
						if(posj == 'A'){	//Agua
							if(st.bikini){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 5;
							}else{

								st_result.coste = st_result.coste + 30;
							}
						}
						else if(posj == 'B'){	//Bosque
							if(st.zapatillas){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 1;
							}else{

								st_result.coste = st_result.coste + 7;
							}
						}
						else if(posj == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 2;	
						}
						else{	//Demás casillas

							st_result.coste = st_result.coste + 1;
						}

						st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
					break;
					case actTURN_SR:	//45º a la der
						if(posj == 'A'){	//Agua
							if(st.bikini){	//Compruebo si tengo bikini

								st_result.coste = st_result.coste + 2;
							}else{

								st_result.coste = st_result.coste + 10;
							}
						}
						else if(posj == 'B'){	//Bosque
							if(st.zapatillas){	//Compruebo si tengo zapatillas

								st_result.coste = st_result.coste + 1;
							}else{

								st_result.coste = st_result.coste + 5;
							}
						}
						else if(posj == 'T'){	//Tierra
									
							st_result.coste = st_result.coste + 1;	
						}
						else{	//Demás casillas

							st_result.coste = st_result.coste + 1;
						}
						st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
					break;

				}
			break;
			case act_CLB_TURN_SR:
				switch(a){
				case actWALK:	//Si la prox casilla es transitable
				sig_ubicacion = NextCasilla(st.jugador);
				if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){
					//COSTES PARA EL COLABORADOR
					if(posc == 'A'){	//Agua
						if(st.bikini_col){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 2;
						}else{

							st_result.coste = st_result.coste + 10;
						}
					}
					else if(posc == 'B'){	//Bosque
						if(st.zapatillas_col){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 1;
						}else{

							st_result.coste = st_result.coste + 5;
						}
					}
					else if(posc == 'T'){	//Tierra
								
						st_result.coste = st_result.coste + 1;	
					}
					else{	//Demás casillas

						st_result.coste = st_result.coste + 1;
					}

					//COSTES PARA EL JUGADOR
					if(posj == 'A'){	//Agua
						if(st.bikini){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 10;
						}else{

							st_result.coste = st_result.coste + 100;
						}
					}
					else if(posj == 'B'){	//Bosque
						if(st.zapatillas){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 15;
						}else{

							st_result.coste = st_result.coste + 50;
						}
					}
					else if(posj == 'T'){	//Tierra
						
						st_result.coste = st_result.coste + 2;	
					}
					else{					//Demás casillas

						st_result.coste = st_result.coste + 1;
					}

					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
					st_result.jugador = sig_ubicacion;
					//Revisamos si la casilla es de bikini o zapatillas
					char casilla = mapa[st.jugador.f][st.jugador.c];
					if(casilla == 'K'){

						st_result.bikini = true;
						st_result.zapatillas = false;
					}
					else if(casilla == 'D'){

						st_result.zapatillas = true;
						st_result.bikini = false;
					}
				}
				break;
				case actRUN:
					sig_ubicacion = NextCasilla(st.jugador);
					if(CasillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){
						
						sig_ubicacion2 = NextCasilla(sig_ubicacion);
						if(CasillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)){
							//COSTES PARA EL COLABORADOR
							if(posc == 'A'){	//Agua
								if(st.bikini_col){	//Compruebo si tengo bikini

									st_result.coste = st_result.coste + 2;
								}else{

									st_result.coste = st_result.coste + 10;
								}
							}
							else if(posc == 'B'){	//Bosque
								if(st.zapatillas_col){	//Compruebo si tengo zapatillas

									st_result.coste = st_result.coste + 1;
								}else{

									st_result.coste = st_result.coste + 5;
								}
							}
							else if(posc == 'T'){	//Tierra
										
								st_result.coste = st_result.coste + 1;	
							}
							else{	//Demás casillas

								st_result.coste = st_result.coste + 1;
							}
							//COSTES PARA EL JUGADOR
							if(posj == 'A'){	//Agua
								if(st.bikini){	//Compruebo si tengo bikini

									st_result.coste = st_result.coste + 15;
								}else{

									st_result.coste = st_result.coste + 150;
								}
							}
							else if(posj == 'B'){	//Bosque
								if(st.zapatillas){	//Compruebo si tengo zapatillas

									st_result.coste = st_result.coste + 25;
								}else{

									st_result.coste = st_result.coste + 75;
								}
							}
							else if(posj == 'T'){	//Tierra
								
								st_result.coste = st_result.coste + 3;	
							}
							else{					//Demás casillas

								st_result.coste = st_result.coste + 1;
							}
							st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
							st_result.jugador = sig_ubicacion2;

							//Revisamos si la casilla es de bikini o zapatillas
							char casilla = mapa[st.jugador.f][st.jugador.c];
							if(casilla == 'K'){

								st_result.bikini = true;
								st_result.zapatillas = false;
							}
							else if(casilla == 'D'){

								st_result.zapatillas = true;
								st_result.bikini = false;
							}
						}
					}
				break;
				case actIDLE:
				//COSTES PARA EL COLABORADOR
					if(posc == 'A'){	//Agua
						if(st.bikini_col){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 2;
						}else{

							st_result.coste = st_result.coste + 10;
							}
						}
					else if(posc == 'B'){	//Bosque
						if(st.zapatillas_col){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 1;
						}else{

							st_result.coste = st_result.coste + 5;
						}
					}
					else if(posc == 'T'){	//Tierra
										
						st_result.coste = st_result.coste + 1;	
						}
					else{	//Demás casillas

						st_result.coste = st_result.coste + 1;
					}

					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
				break;
				case actTURN_L:		//90ª a la izq
					//COSTES PARA EL COLABORADOR
					if(posc == 'A'){	//Agua
						if(st.bikini_col){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 2;
						}else{

							st_result.coste = st_result.coste + 10;
							}
						}
					else if(posc == 'B'){	//Bosque
						if(st.zapatillas_col){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 1;
						}else{

							st_result.coste = st_result.coste + 5;
						}
					}
					else if(posc == 'T'){	//Tierra
										
						st_result.coste = st_result.coste + 1;	
						}
					else{	//Demás casillas

						st_result.coste = st_result.coste + 1;
					}

					//COSTES PARA EL JUGADOR
					if(posj == 'A'){	//Agua
						if(st.bikini){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 5;
						}else{

							st_result.coste = st_result.coste + 30;
						}
					}
					else if(posj == 'B'){	//Bosque
						if(st.zapatillas){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 1;
						}else{

							st_result.coste = st_result.coste + 7;
						}
					}
					else if(posj == 'T'){	//Tierra
								
						st_result.coste = st_result.coste + 2;	
					}
					else{	//Demás casillas

						st_result.coste = st_result.coste + 1;
					}
					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
					st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
				break;
				case actTURN_SR:	//45º a la der
					//COSTES PARA EL COLABORADOR
					if(posc == 'A'){	//Agua
						if(st.bikini_col){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 2;
						}else{

							st_result.coste = st_result.coste + 10;
							}
						}
					else if(posc == 'B'){	//Bosque
						if(st.zapatillas_col){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 1;
						}else{

							st_result.coste = st_result.coste + 5;
						}
					}
					else if(posc == 'T'){	//Tierra
										
						st_result.coste = st_result.coste + 1;	
						}
					else{	//Demás casillas

						st_result.coste = st_result.coste + 1;
					}
					//COSTES PARA EL JUGADOR
					if(posj == 'A'){	//Agua
						if(st.bikini){	//Compruebo si tengo bikini

							st_result.coste = st_result.coste + 2;
						}else{

							st_result.coste = st_result.coste + 10;
						}
					}
					else if(posj == 'B'){	//Bosque
						if(st.zapatillas){	//Compruebo si tengo zapatillas

							st_result.coste = st_result.coste + 1;
						}else{

							st_result.coste = st_result.coste + 5;
						}
					}
					else if(posj == 'T'){	//Tierra
								
						st_result.coste = st_result.coste + 1;	
					}
					else{	//Demás casillas

						st_result.coste = st_result.coste + 1;
					}
					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
					st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
				break;

			}
			break;
		}
	}
	else{	//Accion para el colaborador
		switch(a){
			case act_CLB_WALK:
			sig_ubicacion_col = NextCasilla(st.colaborador);
			if(CasillaTransitable(sig_ubicacion_col, mapa) && !(sig_ubicacion_col.f == st.jugador.f && sig_ubicacion_col.c == st.jugador.c)){

				if(posc == 'A'){	//Agua
					if(st.bikini_col){	//Compruebo si tengo bikini

						st_result.coste = st_result.coste + 10;
					}else{

						st_result.coste = st_result.coste + 100;
					}
				}
				else if(posc == 'B'){	//Bosque
					if(st.zapatillas_col){	//Compruebo si tengo zapatillas

						st_result.coste = st_result.coste + 15;
					}else{

						st_result.coste = st_result.coste + 50;
					}
				}
				else if(posc == 'T'){	//Tierra
					
					st_result.coste = st_result.coste + 2;	
				}
				else{					//Demás casillas

					st_result.coste = st_result.coste + 1;
				}
				st_result.colaborador = sig_ubicacion_col;
				st_result.ultimaOrdenColaborador = act_CLB_WALK;

				//Revisamos si la casilla es de bikini o zapatillas
				char casilla = mapa[st.colaborador.f][st.colaborador.c];
				if(casilla == 'K'){

					st_result.bikini_col = true;
					st_result.zapatillas_col = false;
				}
				else if(casilla == 'D'){

					st_result.zapatillas_col = true;
					st_result.bikini_col = false;
				}
			}

			break;
			case act_CLB_STOP:
				st_result.colaborador = st.colaborador;	//No se modifica nada, se queda quieto
				st_result.ultimaOrdenColaborador = act_CLB_STOP;
			break;

			case act_CLB_TURN_SR:
				if(posc == 'A'){	//Agua
					if(st.bikini_col){	//Compruebo si tengo bikini

						st_result.coste = st_result.coste + 2;
					}else{

						st_result.coste = st_result.coste + 10;
					}
				}
				else if(posc == 'B'){	//Bosque
					if(st.zapatillas_col){	//Compruebo si tengo zapatillas

						st_result.coste = st_result.coste + 1;
					}else{

						st_result.coste = st_result.coste + 5;
					}
				}
				else if(posc == 'T'){	//Tierra
							
					st_result.coste = st_result.coste + 1;	
				}
				else{	//Demás casillas

					st_result.coste = st_result.coste + 1;
				}
				st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
				st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		}
	}


	return st_result;
}
void ComportamientoJugador::AnulaMatriz(vector<vector<unsigned char>> &matriz){
	for(int i=0; i<matriz.size(); i++){
		for(int j=0; j<0 ; j++){

			matriz[i][j]=0;
		}
	}
}

//Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st
//0:No transitada por el plan		1: transitada por el jugador
//2: transitada por el colaborador	3: casilla intermedia a un actRUN del jugador
void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan){
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();

	while(it != plan.end()){

		if((*it != act_CLB_WALK) && (*it != act_CLB_TURN_SR) && (*it != act_CLB_STOP)){
			switch(cst.ultimaOrdenColaborador){

				case act_CLB_WALK:
					cst.colaborador = NextCasilla(cst.colaborador);
					mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
				case act_CLB_TURN_SR:
					cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch(*it){
			
			case actRUN:
				cst.jugador = NextCasilla(cst.jugador);
				mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
				cst.jugador = NextCasilla(cst.jugador);
				mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
			case actWALK:
				cst.jugador = NextCasilla(cst.jugador);
				mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
			case actTURN_SR:
				cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
			case actTURN_L:
				cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				cst.ultimaOrdenColaborador = act_CLB_WALK;
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
			case act_CLB_STOP:
				cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

void ComportamientoJugador::PintaPlan(const list<Action> &plan){
	auto it = plan.begin();
	while(it != plan.end()){
		if(*it == actWALK){

			cout<< "W ";
		}
		else if(*it == actRUN){

			cout<< "R ";
		}
		else if(*it == actTURN_SR){

			cout<< "r ";
		}
		else if(*it == actTURN_L){

			cout<< "L ";
		}
		else if(*it == act_CLB_WALK){

			cout<< "cW ";
		}
		else if(*it == act_CLB_TURN_SR){

			cout<< "cr ";
		}
		else if(*it == act_CLB_STOP){

			cout<< "cS ";
		}
		else if(*it == actIDLE){

			cout<< "I ";
		}
		else{

			cout<< "-_ ";
		}
		it++;
	}

	cout << " (" <<plan.size() << " acciones)" << endl;
}

list<Action> ComportamientoJugador::AnchuraSoloJugador_V3(const stateN0 & inicio, const ubicacion & final ,
 const vector<vector<unsigned char>> & mapa){

	nodeN0 current_node;
	list<nodeN0> frontier;	
	set<nodeN0> explored;	
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (current_node.st.jugador.f == final.f && current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while(!frontier.empty() && !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		//Generar hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if(child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c){

			current_node = child_walk;
			SolutionFound = true;				
		}
		else if(explored.find(child_walk) == explored.end()){

			frontier.push_back(child_walk);
		}

		if(!SolutionFound){
			//Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if(child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c){

				current_node = child_run;
				SolutionFound = true;				
			}
			else if(explored.find(child_run) == explored.end()){

				frontier.push_back(child_run);
			}
		}

		if(!SolutionFound){
			//Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(explored.find(child_turnl) == explored.end()){

				frontier.push_back(child_turnl);
			}

			//Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if(explored.find(child_turnsr) == explored.end()){

				frontier.push_back(child_turnsr);
			}
		}

		if(!SolutionFound && !frontier.empty()){

			current_node = frontier.front();
			while(!frontier.empty() && explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty()){

					current_node = frontier.front();
				}
			}
		}
	}

	if(SolutionFound){

		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
 }


bool ComportamientoJugador::jugadorVeColaborador(const stateN1 & st){
	Orientacion orientacion = st.jugador.brujula;
	ubicacion ubj = st.jugador;
	ubicacion ubc = st.colaborador;
	switch(orientacion){
		case norte: 
			if((ubj.f - 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 1 == ubc.c)||
			(ubj.f - 3  == ubc.f && ubj.c  == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 3  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f - 2  == ubc.f && ubj.c == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 1 == ubc.c)){

				return true;
			}
		break;
		case noreste:
			if((ubj.f - 3  == ubc.f && ubj.c  == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 2 == ubc.c)||
			(ubj.f - 3  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 3 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 1 == ubc.c) || 
			(ubj.f - 2  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 1  == ubc.f && ubj.c == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c + 1 == ubc.c)){

				return true;
			}
			
		break;
		case este:
		 	if((ubj.f - 3  == ubc.f && ubj.c  + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 3 == ubc.c)||
			(ubj.f == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c + 3 == ubc.c) || 
			(ubj.f + 3 == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f - 1 == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 1  == ubc.f && ubj.c  + 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f  + 1 == ubc.f && ubj.c + 1 == ubc.c)){

				return true;
			}
		break;
		case sureste:
			if((ubj.f == ubc.f && ubj.c  + 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c + 3 == ubc.c)||
			(ubj.f + 3  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c + 1 == ubc.c) || 
			(ubj.f + 3 == ubc.f && ubj.c == ubc.c) || (ubj.f  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f + 2  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f  + 2 == ubc.f && ubj.c == ubc.c) || 
			(ubj.f == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c == ubc.c)){

				return true;
			}
		break;
		case sur:
			if ((ubj.f + 3 == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c - 1 == ubc.c) ||
			(ubj.f + 3 == ubc.f && ubj.c == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c + 2 == ubc.c) ||
			(ubj.f + 3 == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f + 2 == ubc.f && ubj.c == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c + 2 == ubc.c) ||
			(ubj.f + 1 == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c + 1 == ubc.c)) {
        
			return true;
    		}	
		break;
		case suroeste:
			if((ubj.f + 3  == ubc.f && ubj.c  == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c - 2 == ubc.c)||
			(ubj.f + 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 3 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f + 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f  == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f + 1  == ubc.f && ubj.c == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c - 1 == ubc.c)){

				return true;
			}
		
		break;
		case oeste: 
			if((ubj.f + 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 3 == ubc.c)||
			(ubj.f == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 3 == ubc.c) || 
			(ubj.f - 3 == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 2 == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f + 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  - 1 == ubc.f && ubj.c - 1 == ubc.c)){

				return true;
			}
		
		break;
		case noroeste:
			if((ubj.f == ubc.f && ubj.c  - 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 3 == ubc.c)||
			(ubj.f - 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f - 3 == ubc.f && ubj.c == ubc.c) || (ubj.f  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f - 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  - 2 == ubc.f && ubj.c == ubc.c) || 
			(ubj.f == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f - 1 == ubc.f && ubj.c == ubc.c)){

				return true;
			}
		break;
	}
	return false;
}

bool ComportamientoJugador::jugadorVeColaboradorN3(const stateN3 & st){
	Orientacion orientacion = st.jugador.brujula;
	ubicacion ubj = st.jugador;
	ubicacion ubc = st.colaborador;
	switch(orientacion){
		case norte: 
			if((ubj.f - 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 1 == ubc.c)||
			(ubj.f - 3  == ubc.f && ubj.c  == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 3  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f - 2  == ubc.f && ubj.c == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 1 == ubc.c)){

				return true;
			}
		break;
		case noreste:
			if((ubj.f - 3  == ubc.f && ubj.c  == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c + 2 == ubc.c)||
			(ubj.f - 3  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 3 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 1 == ubc.c) || 
			(ubj.f - 2  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 1  == ubc.f && ubj.c == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c + 1 == ubc.c)){

				return true;
			}
			
		break;
		case este:
		 	if((ubj.f - 3  == ubc.f && ubj.c  + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c + 3 == ubc.c)||
			(ubj.f == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c + 3 == ubc.c) || 
			(ubj.f + 3 == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f - 1 == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f - 1  == ubc.f && ubj.c  + 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f  + 1 == ubc.f && ubj.c + 1 == ubc.c)){

				return true;
			}
		break;
		case sureste:
			if((ubj.f == ubc.f && ubj.c  + 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c + 3 == ubc.c)||
			(ubj.f + 3  == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c + 1 == ubc.c) || 
			(ubj.f + 3 == ubc.f && ubj.c == ubc.c) || (ubj.f  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 2 == ubc.c) || 
			(ubj.f + 2  == ubc.f && ubj.c + 2 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f  + 2 == ubc.f && ubj.c == ubc.c) || 
			(ubj.f == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c == ubc.c)){

				return true;
			}
		break;
		case sur:
			if ((ubj.f + 3 == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c - 1 == ubc.c) ||
			(ubj.f + 3 == ubc.f && ubj.c == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 3 == ubc.f && ubj.c + 2 == ubc.c) ||
			(ubj.f + 3 == ubc.f && ubj.c + 3 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f + 2 == ubc.f && ubj.c == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c + 1 == ubc.c) || (ubj.f + 2 == ubc.f && ubj.c + 2 == ubc.c) ||
			(ubj.f + 1 == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c + 1 == ubc.c)) {
        
			return true;
    		}	
		break;
		case suroeste:
			if((ubj.f + 3  == ubc.f && ubj.c  == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f + 3  == ubc.f && ubj.c - 2 == ubc.c)||
			(ubj.f + 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 3 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f + 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f  == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f + 1  == ubc.f && ubj.c == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c - 1 == ubc.c)){

				return true;
			}
		
		break;
		case oeste: 
			if((ubj.f + 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 1  == ubc.f && ubj.c - 3 == ubc.c)||
			(ubj.f == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 3 == ubc.c) || 
			(ubj.f - 3 == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f + 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f + 1 == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 2 == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f + 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  - 1 == ubc.f && ubj.c - 1 == ubc.c)){

				return true;
			}
		
		break;
		case noroeste:
			if((ubj.f == ubc.f && ubj.c  - 3 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 3 == ubc.c)||
			(ubj.f - 3  == ubc.f && ubj.c - 3 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 3  == ubc.f && ubj.c - 1 == ubc.c) || 
			(ubj.f - 3 == ubc.f && ubj.c == ubc.c) || (ubj.f  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 2 == ubc.c) || 
			(ubj.f - 2  == ubc.f && ubj.c - 2 == ubc.c) || (ubj.f - 2  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f  - 2 == ubc.f && ubj.c == ubc.c) || 
			(ubj.f == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f - 1  == ubc.f && ubj.c - 1 == ubc.c) || (ubj.f - 1 == ubc.f && ubj.c == ubc.c)){

				return true;
			}
		break;
	}
	return false;
}
list <Action> ComportamientoJugador::AnchuraN1(const stateN1 & inicio, const ubicacion & final , const vector<vector<unsigned char>> & mapa){
	nodeN1 current_node;
	list<nodeN1> frontier;	
	set<nodeN1> explored;	
	list<Action> plan;
	current_node.st = inicio;

    bool SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);	
	frontier.push_back(current_node);
	
	while(!frontier.empty() && !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		/*if(current_node.secuencia.size() < 10)
			PintaPlan(current_node.secuencia);*/

		//Si el jugador ve al colaborador, se crean los hijos del colaborador
		if(jugadorVeColaborador(current_node.st)){
			//Generar hijo clbWALK
			nodeN1 child_cwalk = current_node;
			child_cwalk.st = applyN1(act_CLB_WALK, current_node.st, mapa);
			child_cwalk.secuencia.push_back(act_CLB_WALK);
			if((child_cwalk.st.colaborador.f == final.f && child_cwalk.st.colaborador.c == final.c)){

				current_node = child_cwalk;
				SolutionFound = true;				
			}
			else if(explored.find(child_cwalk) == explored.end()){

				frontier.push_back(child_cwalk);
			}

			if(!SolutionFound){
				//Generar hijo act_CLB_STOP
				nodeN1 child_stop = current_node;
				child_stop.st = applyN1(act_CLB_STOP, current_node.st, mapa);
				child_stop.secuencia.push_back(act_CLB_STOP);
				//child_stop.st.ultimaOrdenColaborador = act_CLB_STOP;
				if(explored.find(child_stop) == explored.end()){

					frontier.push_back(child_stop);
				}
			
				//Generar hijo act_CLB_TURN_SR
				nodeN1 child_turnsr = current_node;
				child_turnsr.st = applyN1(act_CLB_TURN_SR, current_node.st, mapa);
				child_turnsr.secuencia.push_back(act_CLB_TURN_SR);
				//child_turnsr.st.ultimaOrdenColaborador = act_CLB_TURN_SR;
				if(explored.find(child_turnsr) == explored.end()){

					frontier.push_back(child_turnsr);
				}
			}
		}
		
		if(!SolutionFound){
			//Generar hijo actWALK
			nodeN1 child_walk = current_node;
			child_walk.st = applyN1(actWALK, current_node.st, mapa);
			child_walk.secuencia.push_back(actWALK);
			if((child_walk.st.colaborador.f == final.f && child_walk.st.colaborador.c == final.c)){

				current_node = child_walk;
				SolutionFound = true;				
			}
			else if(explored.find(child_walk) == explored.end()){

				frontier.push_back(child_walk);
			}
		}

		if(!SolutionFound){
			//Generar hijo actRUN
			nodeN1 child_run = current_node;
			child_run.st = applyN1(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if((child_run.st.colaborador.f == final.f && child_run.st.colaborador.c == final.c)){

				current_node = child_run;
				SolutionFound = true;				
			}
			else if(explored.find(child_run) == explored.end()){

				frontier.push_back(child_run);
			}
		}

		if(!SolutionFound){
			//Generar hijo actTURN_L
			nodeN1 child_turnl = current_node;
			child_turnl.st = applyN1(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(explored.find(child_turnl) == explored.end()){

				frontier.push_back(child_turnl);
			}

			//Generar hijo actTURN_SR
			nodeN1 child_turnsr = current_node;
			child_turnsr.st = applyN1(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if(explored.find(child_turnsr) == explored.end()){

				frontier.push_back(child_turnsr);
			}
		}


		if(!SolutionFound && !frontier.empty()){

			current_node = frontier.front();
			while(!frontier.empty() && explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty()){

					current_node = frontier.front();
				}
			}
		}
	}

	if(SolutionFound){

		plan = current_node.secuencia;
		cout << "Encontrado un plan para el colaborador: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}

struct CompareNodeCost {
    bool operator()(const nodeN2 &a, const nodeN2 &b) const {
        // Comparar los costos de los nodos
        return a.st.coste > b.st.coste; // Ordenar de menor a mayor
    }
};

list <Action> ComportamientoJugador::DijkstraN2(const stateN2 & inicio, const ubicacion & final, const vector<vector<unsigned char>> & mapa){

	nodeN2 current_node;
	priority_queue<nodeN2, vector<nodeN2>, CompareNodeCost> frontier;	
	set<nodeN2> explored;	
	list<Action> plan;
	current_node.st = inicio;

    bool SolutionFound = (current_node.st.jugador.f == final.f && current_node.st.jugador.c == final.c);	
	frontier.push(current_node);

	while(!frontier.empty() && !SolutionFound){
		current_node = frontier.top();
		frontier.pop();
		explored.insert(current_node);
		
		//Generar hijo actWALK
		nodeN2 child_walk = current_node;
		child_walk.st = applyN2(actWALK, current_node.st, mapa);
		if(explored.find(child_walk) == explored.end()){

			child_walk.secuencia.push_back(actWALK);
			frontier.push(child_walk);
		}
		
		//Generar hijo actRUN
		if(!SolutionFound){
			nodeN2 child_run = current_node;
			child_run.st = applyN2(actRUN, current_node.st, mapa);
			if(explored.find(child_run) == explored.end()){

				child_run.secuencia.push_back(actRUN);
				frontier.push(child_run);
			}
		}
		
		//Generar hijo actTURN_L
		if(!SolutionFound){
			nodeN2 child_left = current_node;
			child_left.st = applyN2(actTURN_L, current_node.st, mapa);
			if(explored.find(child_left) == explored.end()){

				child_left.secuencia.push_back(actTURN_L);
				frontier.push(child_left);
			}

			//Generar hijo actTURN_SR
			nodeN2 child_turnsr = current_node;
			child_turnsr.st = applyN2(actTURN_SR, current_node.st, mapa);
			if(explored.find(child_turnsr) == explored.end()){

				child_turnsr.secuencia.push_back(actTURN_SR);
				frontier.push(child_turnsr);
			}
		}

		if(!SolutionFound && !frontier.empty()){
			
			current_node = frontier.top();
			while(!frontier.empty() && explored.find(current_node) != explored.end()){
				frontier.pop();
				if(!frontier.empty()){

					current_node = frontier.top();
				}
			}
		}
		SolutionFound = (current_node.st.jugador.f == final.f && current_node.st.jugador.c == final.c);
		
	}

	if(SolutionFound){

		plan = current_node.secuencia;
		cout << "Encontrado un plan ";
		PintaPlan(current_node.secuencia);
		cout<< "Coste del plan: " << current_node.st.coste << endl; 
	}
	return plan;
}

int ComportamientoJugador::valorHeuristico(const stateN3 & st,const ubicacion & final,bool tipo){
	int valor = 0;
	if(!tipo){

		valor = max(abs(st.jugador.f - st.colaborador.f),abs(st.jugador.c - st.colaborador.c));
	}
	else{

		valor = max(abs(st.colaborador.f - final.f),abs(st.colaborador.c - final.c));
	}
	
	return valor;
}

struct CompareNode {
    bool operator()(const nodeN3 &a, const nodeN3 &b) const {
        // Comparar los costos de los nodos
		int sumaA = a.st.coste + a.st.heuristica;
		int sumaB = b.st.coste + b.st.heuristica;

		if(sumaA > sumaB){  // Ordenar de menor a mayor

			return true;
		}
		else if( sumaA == sumaB && a.secuencia.size() > b.secuencia.size()){

			return true;
		}
		else{

			return false;
		}
    }
};

list <Action> ComportamientoJugador::AestrellaN3(const stateN3 & inicio, const ubicacion & final, const vector<vector<unsigned char>> & mapa){
	nodeN3 current_node;
	priority_queue<nodeN3, vector<nodeN3>, CompareNode> frontier;	
	set<nodeN3> explored;	
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);	
	frontier.push(current_node);

	while(!frontier.empty() && !SolutionFound){
		current_node = frontier.top();
		frontier.pop();
		explored.insert(current_node);

		/*if(current_node.secuencia.size() < 4)
			PintaPlan(current_node.secuencia);*/

		if(jugadorVeColaboradorN3(current_node.st)){
			//Generar hijo act_CLB_WALK
			nodeN3 child_cwalk = current_node;
			child_cwalk.st = applyN3(act_CLB_WALK, current_node.st, mapa);
			child_cwalk.st.heuristica = valorHeuristico(current_node.st,final,true);
			if(explored.find(child_cwalk) == explored.end()){

				child_cwalk.secuencia.push_back(act_CLB_WALK);
				frontier.push(child_cwalk);
			}

			if(!SolutionFound){
				//Generar hijo act_CLB_STOP
				nodeN3 child_stop = current_node;
				child_stop.st = applyN3(act_CLB_STOP,current_node.st, mapa);
				child_stop.st.heuristica = valorHeuristico(child_stop.st,final,true);
				if(explored.find(child_stop) == explored.end()){

					child_stop.secuencia.push_back(act_CLB_STOP);
					frontier.push(child_stop);
				}

				//Generar hijo act_CLB_TURN_SR
				nodeN3 child_cturn = current_node;
				child_cturn.st = applyN3(act_CLB_TURN_SR,current_node.st, mapa);
				child_cturn.st.heuristica = valorHeuristico(current_node.st,final,true);
				if(explored.find(child_cturn) == explored.end()){

					child_cturn.secuencia.push_back(act_CLB_TURN_SR);
					frontier.push(child_cturn);
				}
			}
		}
		if(!SolutionFound){
			//Generar hijo actWALK
			nodeN3 child_walk = current_node;
			child_walk.st = applyN3(actWALK, current_node.st, mapa);
			child_walk.st.heuristica = valorHeuristico(current_node.st,final,true);
			if(explored.find(child_walk) == explored.end()){

				child_walk.secuencia.push_back(actWALK);
				frontier.push(child_walk);
			}
		}

		//Generar hijo actRUN
		if(!SolutionFound){
			nodeN3 child_run = current_node;
			child_run.st = applyN3(actRUN, current_node.st, mapa);
			child_run.st.heuristica = valorHeuristico(current_node.st,final,true);
			if(explored.find(child_run) == explored.end()){

				child_run.secuencia.push_back(actRUN);
				frontier.push(child_run);
			}
		}

		//Generar hijo actIDLE
		if(!SolutionFound){
			nodeN3 child_idle = current_node;
			child_idle.st = applyN3(actIDLE, current_node.st, mapa);
			child_idle.st.heuristica = valorHeuristico(current_node.st,final,true);
			if(explored.find(child_idle) == explored.end()){

				child_idle.secuencia.push_back(actIDLE);
				frontier.push(child_idle);
			}
		}
		
		
		//Generar hijo actTURN_L
		if(!SolutionFound){
			nodeN3 child_left = current_node;
			child_left.st = applyN3(actTURN_L, current_node.st, mapa);
			child_left.st.heuristica = valorHeuristico(current_node.st,final,true);
			if(explored.find(child_left) == explored.end()){

				child_left.secuencia.push_back(actTURN_L);
				frontier.push(child_left);
			}

			//Generar hijo actTURN_SR
			nodeN3 child_turnsr = current_node;
			child_turnsr.st = applyN3(actTURN_SR, current_node.st, mapa);
			child_turnsr.st.heuristica = valorHeuristico(current_node.st,final,true);
			if(explored.find(child_turnsr) == explored.end()){

				child_turnsr.secuencia.push_back(actTURN_SR);
				frontier.push(child_turnsr);
			}
		}


		if(!SolutionFound && !frontier.empty()){
			
			current_node = frontier.top();
			while(!frontier.empty() && explored.find(current_node) != explored.end()){
				frontier.pop();
				if(!frontier.empty()){

					current_node = frontier.top();
				}
			}
		}

		SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);	
	}

	if(SolutionFound){

		plan = current_node.secuencia;
		cout << "Encontrado un plan ";
		PintaPlan(current_node.secuencia);
		cout<< "Coste del plan: " << current_node.st.coste << endl; 
	}
	return plan;
}