#ifndef CRUZAMENTO_h
#define CRUZAMENTO_h
#include "Semaforo.h"
#include "Pedestre.h"
#include "Via.h"

struct Estagio
{
    unsigned long tempo = 0;
    int numVias = 0;
    Via **vias = NULL;
    char *states = NULL;
};

Estagio* e;
Via* via;
char state;

class Cruzamento
{
private:
    Estagio **estagios;
    int numEstagios = 0;
    unsigned long tempoAmarelo;
    unsigned long tempoOffset;
    int maxEstagios = 0;
    int currentEstagio = 0;
    char previousState = 'y'; // yellow, offset, next estagio

    unsigned long currentMillis = 0;

    bool forcedMode = false;

    unsigned long percentage(unsigned int millisVal, int fraction){
        if (fraction > 100) return millisVal;
        if(fraction <= 0) return 0;

        return millisVal / 100.0 * fraction;
    }

public:
    Cruzamento(int tempoAmarelo, int tempoOffset, int maxEstagios)
    {
        this->maxEstagios = maxEstagios;
        this->tempoAmarelo = tempoAmarelo;
        this->tempoOffset = tempoOffset;
        estagios = new Estagio*[maxEstagios];
        for (int i = 0; i < maxEstagios; i++) {
            estagios[i] = NULL;
        }
    };
    ~Cruzamento(){
        for (int i = 0; i < numEstagios; i++) {
            delete[] estagios[i]->states;

            for (int i = 0; i < estagios[i]->numVias; i++) {
                delete estagios[i]->vias[i];
            }

            delete[] estagios[i]->vias;
        }
        delete[] estagios;
    }

    void attachVia(int estagio, Via *via, char state)
    {
        if (estagio <= numEstagios && estagio >= 0)
        {
            if (estagios[estagio] == NULL || estagio == numEstagios) {
                Serial.println("Erro attachVia: unnitialized Estagio");
            }

            e = estagios[estagio];
            e->vias[e->numVias] = via;
            e->states[e->numVias] = state;
            e->numVias++;
            // Serial.println(e->numVias);
        }else{
            Serial.println("Erro attachVia: estagio out of range");
        }
    };

    void setEstagioForced(int estagio){
        forcedMode = true;
        setEstagio(estagio);
    }
    void setEstagio(int estagio){
        if (estagio >= 0 || estagio < numEstagios) {
            currentEstagio = estagio;
        }
    }

    void forceNextEstagio(){
        forcedMode = true;
        // skipEstagio();
    }
    void skipEstagio(){
        currentEstagio++;
        if (currentEstagio >= numEstagios) {
            currentEstagio = 0;
        }
    }

    void configureEstagio(int estagio, int tempo){
        if (estagios[estagio] == NULL || estagio == numEstagios) {
            configureEstagio(estagio, tempo, 0);
        }else{
            Serial.println("Erro configureEstagio: estagio out of range");
        }
    }
    void configureEstagio(int estagio, int tempo, int maxVias)
    {
        if (estagio <= numEstagios && estagio >= 0 && estagio < maxEstagios)
        {
            if (estagios[estagio] == NULL || estagio == numEstagios) {
                estagios[estagio] = new Estagio;
                numEstagios++;
            }
            e = estagios[estagio];
            e->tempo = tempo;

            if (maxVias > 0) {
                e->vias = new Via*[maxVias];
                e->states = new char[maxVias];
                for (int i = 0; i < maxVias; i++) {
                    e->vias[i] = NULL;
                    e->states[i] = 'r';
                }
            }

            Serial.print("Tempo - estagio ");
            Serial.print(estagio);
            Serial.print(": ");
            Serial.println(tempo);
        }else{
            Serial.println("Erro configureEstagio: estagio out of range");
        }
        e = estagios[currentEstagio];
    }

    void run()
    {
        // if ((millis() - currentMillis) % 1000 < 100)
        // {
        //     Serial.print(( forcedMode ? percentage(e->tempo, 30) : e->tempo ));
        //     Serial.print("   -   ");
        //     Serial.println(millis() - currentMillis);
        // }
        
        if (previousState == 'n') {
            for (int i = 0; i < e->numVias; i++) {
                via = e->vias[i];
                state = e->states[i];

                if (state == 'r' &&
                    via->getNumPedestres() > 0 &&
                    millis() - currentMillis > ( forcedMode ? percentage(e->tempo, 30) : e->tempo ) - via->pedestreInterval) {
                    for (int j = 0; j < via->getNumPedestres(); j++) {
                        via->getPedestre(j)->blink();
                    }
                }
            }

            if (millis() - currentMillis > ( forcedMode ? percentage(e->tempo, 30) : e->tempo )) {

                Serial.println("Passou Amarelo");
                currentMillis = millis();
                previousState = 'y';

                for (int i = 0; i < e->numVias; i++)
                {
                    via = e->vias[i];
                    char state = e->states[i];

                    for (int j = 0; j < via->getNumPedestres(); j++) {
                        via->getPedestre(j)->red();
                    }

                    for (int j = 0; j < via->getNumSemaforos(); j++)
                    {
                        if (state == 'g') {
                            via->getSemaforo(j)->setState('y');
                        }
                    }
                }
            }
        }

        if (millis() - currentMillis > tempoAmarelo && previousState == 'y') {
            Serial.println("Passou Offset/Vermelho");
            currentMillis = millis();
            previousState = 'o';

            for (int i = 0; i < e->numVias; i++)
            {
                via = e->vias[i];
                char state = e->states[i];

                for (int j = 0; j < via->getNumSemaforos(); j++) {
                    if (state == 'g') {
                        via->getSemaforo(j)->setState('r');
                    }
                }
            }
        }


        if (millis() - currentMillis > tempoOffset && previousState == 'o') {
            Serial.println("Passou Next Estagio");
            currentMillis = millis();
            previousState = 'n';
            currentEstagio++;

            if (currentEstagio >= numEstagios)
                currentEstagio = 0;


            e = estagios[currentEstagio];

            for (int i = 0; i < e->numVias; i++) {
                via = e->vias[i];
                state = e->states[i];

                for (int j = 0; j < via->getNumSemaforos(); j++) {
                    via->getSemaforo(j)->setState(state);
                }

                for (int j = 0; j < via->getNumPedestres(); j++) {
                    via->getPedestre(j)->setState(state);
                }
                
            }

            forcedMode = false;

        }

    };
};
#endif
