#ifndef VIA_h
#define VIA_h
#include "Semaforo.h"
#include "Pedestre.h"

class Via
{
private:
    Semaforo **semaforos = NULL;
    Pedestre **pedestres = NULL;
    int numPedestres = 0;
    int numSemaforos = 0;

    int maxSemaforos = 0;

public:
    int pedestreInterval = 500;

    Via(int maxSemaforos)
    {
        this->maxSemaforos = maxSemaforos;
        semaforos = new Semaforo*[maxSemaforos];
        pedestres = new Pedestre*[maxSemaforos];
        for (int i = 0; i < maxSemaforos; i++) {
            semaforos[i] = NULL;
            pedestres[i] = NULL;
        }
    };

    ~Via(){
        for (int i = 0; i < numSemaforos; i++)
        {
            delete semaforos[i];
        }
        delete[] semaforos;

        for (int i = 0; i < numPedestres; i++)
        {
            delete pedestres[i];
        }
        delete[] pedestres;
    }

    void configure(int pedestreInterval){
        this->pedestreInterval = pedestreInterval;
    }

    void attachSemaforo(Semaforo *semaforo) {
        if (numSemaforos >= maxSemaforos) {
            Serial.println("Erro attachSemaforo: Via is full");
            return;
        }

        semaforos[numSemaforos] = semaforo;
        numSemaforos++;
    };

    void attachPedestre(Pedestre *pedestre) {
        if (numPedestres >= maxSemaforos) {
            Serial.println("Erro attachPedestre: Via is full");
            return;
        }
        
        // attachSemaforo(pedestre);
        pedestres[numPedestres] = pedestre;
        numPedestres++;
    };

    // Get methods ---------------

    Semaforo* getSemaforo(int i){
        return semaforos[i];
    };

    Pedestre* getPedestre(int i){
        return pedestres[i];
    };
    int getNumSemaforos(){
        return numSemaforos;
    };
    int getNumPedestres(){
        return numPedestres;
    };


};
#endif