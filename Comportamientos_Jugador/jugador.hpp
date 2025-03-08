#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct stateN0{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x ) const{
    if(jugador == x.jugador && colaborador.c == x.colaborador.c && colaborador.f == x.colaborador.f){

      return true;
    }
    else{

      return false;
    }
  }
};

//Definición del tipo nodo del nivel 0

struct nodeN0{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0 &n) const{

    return st == n.st;
  }

  bool operator<(const nodeN0 &b) const{
    if(st.jugador.f < b.st.jugador.f){  //Más a la izq

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c){ //Más arriba

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula){

      return true;
    }
    else{

      return false;
    }
  }
};

struct stateN1{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN1 &x ) const{
    if(jugador == x.jugador && colaborador.c == x.colaborador.c && colaborador.f == x.colaborador.f){

      return true;
    }
    else{

      return false;
    }
  }
};

//Definición del tipo nodo del nivel 1

struct nodeN1{
  stateN1 st;
  list<Action> secuencia;

  bool operator==(const nodeN1 &n) const{

    return st == n.st;
  }

  bool operator<(const nodeN1 &b) const{
    if(st.jugador.f < b.st.jugador.f){  //Más a la izq

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c){ //Más arriba

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && 
    st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f < b.st.colaborador.f){  

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador.f && st.colaborador.c < b.st.colaborador.c){ 

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula < b.st.colaborador.brujula){

      return true;
    }
    else{

      return false;
    }

  }
};

struct stateN2{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  int coste;  //coste de llegar a ese nodo (no influye para que 2 nodos son iguales, el mismo nodo dependiendo del momento, puede tener un coste u otro)
  bool bikini;  //para saber si lleva biquini o no
  bool zapatillas; //para saber si lleva zapatillas o no

  bool operator==(const stateN2 &x ) const{
    if(jugador == x.jugador && colaborador.c == x.colaborador.c && colaborador.f == x.colaborador.f && bikini == x.bikini && zapatillas == x.zapatillas){

      return true;
    }
    else{

      return false;
    }
  }

  
};

//Definición del tipo nodo del nivel 2

struct nodeN2{
  stateN2 st;
  list<Action> secuencia;

  bool operator==(const nodeN2 &n) const{

    return st == n.st;
  }

  bool operator<(const nodeN2 &b) const{
    if(st.jugador.f < b.st.jugador.f){  //Más a la izq

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c){ //Más arriba

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula
    && st.bikini < b.st.bikini){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula
    && st.bikini == b.st.bikini && st.zapatillas < b.st.zapatillas){

      return true;
    }
    else{

      return false;
    }
  }
};

struct stateN3{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  int coste;  //coste de llegar a ese nodo (no influye para que 2 nodos son iguales, el mismo nodo dependiendo del momento, puede tener un coste u otro)
  int heuristica; // heuristica del algoritmo de A*
  bool bikini;  //para saber si el jugador lleva biquini o no
  bool zapatillas; //para saber si  el jugador lleva zapatillas o no
  bool bikini_col;  //para saber ei el colaborador lleva bikini o no
  bool zapatillas_col; //para saber si el colaborador lleva zapatillas o no

  bool operator==(const stateN3 &x ) const{
    if(jugador == x.jugador && colaborador.c == x.colaborador.c && colaborador.f == x.colaborador.f && bikini == x.bikini && zapatillas == x.zapatillas 
     && bikini_col == x.bikini_col && zapatillas_col == x.zapatillas_col){

      return true;
    }
    else{

      return false;
    }
  }
};

struct nodeN3{
  stateN3 st;
  list<Action> secuencia;

  bool operator==(const nodeN3 &n) const{

    return st == n.st;
  }

  bool operator<(const nodeN3 &b) const{
    if(st.jugador.f < b.st.jugador.f){  //Más a la izq

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c){ //Más arriba

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && 
    st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f < b.st.colaborador.f){  

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador.f && st.colaborador.c < b.st.colaborador.c){ 

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula < b.st.colaborador.brujula){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula == b.st.colaborador.brujula && 
    st.bikini < b.st.bikini){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula == b.st.colaborador.brujula && 
    st.bikini == b.st.bikini && st.zapatillas < b.st.zapatillas){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula == b.st.colaborador.brujula && 
    st.bikini == b.st.bikini && st.zapatillas == b.st.zapatillas && st.bikini_col < b.st.bikini_col){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula == b.st.colaborador.brujula && 
    st.bikini == b.st.bikini && st.zapatillas == b.st.zapatillas && st.bikini_col == b.st.bikini_col && st.zapatillas_col < b.st.zapatillas_col){

      return true;
    }
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && 
    st.colaborador.f == b.st.colaborador .f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula == b.st.colaborador.brujula && 
    st.bikini == b.st.bikini && st.zapatillas == b.st.zapatillas && st.bikini_col == b.st.bikini_col && st.zapatillas_col == b.st.zapatillas_col && 
    st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador){

      return true;
    }
    else{

      return false;
    }
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado solo para el nivel 4
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado para niveles 0, 1 ,2 y 3
      hayPlan = false;
      primero = true;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    // Declarar Variables de Estado

    list<Action> plan;       //Almacena el plan de ejecucion
    bool hayPlan;           //Si esta en true, indica que tenemos un plan a seguir
    stateN0 c_state;        //Estado actual tanto del jugador como del colaborador
    stateN1 col_state;
    stateN2 c_state2;
    stateN3 c_state3;
    ubicacion goal;         //Ubicacion de la casilla objetivo
    bool primero;


    //Funciones
    bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
    ubicacion NextCasilla(const ubicacion &pos);
    stateN0 apply(const Action & a, const stateN0 & st, const vector<vector<unsigned char>> &mapa);
    stateN1 applyN1(const Action & a, const stateN1 & st, const vector<vector<unsigned char>> &mapa);
    stateN2 applyN2(const Action & a, const stateN2 & st, const vector<vector<unsigned char>> &mapa);
    stateN3 applyN3(const Action & a, const stateN3 & st, const vector<vector<unsigned char>> &mapa);
    int valorHeuristico(const stateN3 & st,const ubicacion & final,bool tipo);
    void AnulaMatriz(vector<vector<unsigned char>> &matriz);
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    bool Find(const stateN0 &item, const list<nodeN0> &lista);  //Find para node0
    void PintaPlan(const list<Action> &plan);
    bool jugadorVeColaborador(const stateN1 & st);
    bool jugadorVeColaboradorN3(const stateN3 & st);
    //bool visionColaborador(const stateN1)
    //Funciones sobre algoritmos
    list <Action> AnchuraSoloJugador_V3(const stateN0 & inicio, const ubicacion & final , const vector<vector<unsigned char>> & mapa); //Funcional con set
    list <Action> AnchuraN1(const stateN1 & inicio, const ubicacion & final , const vector<vector<unsigned char>> & mapa); 
    list <Action> DijkstraN2(const stateN2 & inicio, const ubicacion & final, const vector<vector<unsigned char>> & mapa);
    list <Action> AestrellaN3(const stateN3 & inicio, const ubicacion & final, const vector<vector<unsigned char>> & mapa);
};

#endif
