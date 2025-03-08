#ifndef COMPORTAMIENTOCOLABORADOR_H
#define COMPORTAMIENTOCOLABORADOR_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoColaborador : public Comportamiento{

  public:
    ComportamientoColaborador(unsigned int size = 0) : Comportamiento(size){
      paso=0;
      last_action = act_CLB_STOP;
    }
    ComportamientoColaborador(const ComportamientoColaborador & comport) : Comportamiento(comport){}
    ~ComportamientoColaborador(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoColaborador * clone(){return new ComportamientoColaborador(*this);}

  private:
    int paso;
    Action last_action;

};



#endif
