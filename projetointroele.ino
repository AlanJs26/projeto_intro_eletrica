#include "Semaforo.h"
#include "Cruzamento.h"
#include "Via.h"
#include "Pedestre.h"

Semaforo* s1 = new Semaforo(9,10,11);
Pedestre* p1 = new Pedestre(4,6,200);
// Semaforo* s2 = new Semaforo(4,5,6);

Cruzamento* cruzamento = new Cruzamento(1000, 1000, 2);

void setup() {
  Serial.begin(9600);

  Via* via1 = new Via(1);
  via1->attachSemaforo(s1);

  Via* via2 = new Via(1);
  via2->configure(3000);
  // via2->attachSemaforo(s2);
  via2->attachPedestre(p1);
  
  cruzamento->configureEstagio(0, 10000, 2);
  cruzamento->attachVia(0, via1, 'g');
  cruzamento->attachVia(0, via2, 'r');
  

  cruzamento->configureEstagio(1, 10500, 2);
  cruzamento->attachVia(1, via1, 'r');
  cruzamento->attachVia(1, via2, 'g');


  // s1->off();
  
  // s2->off();
  
}

void loop() {
  cruzamento->run();
  char c = Serial.read();

  if (c == 'a')
  {
    cruzamento->forceNextEstagio();
    Serial.println("Forcing Estagio skip");
  }
  
  // p1->blink();
}
