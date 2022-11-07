#ifndef PEDESTRE_h
#define PEDESTRE_h
#include "Semaforo.h"

class Pedestre : public Semaforo
{
private:
  unsigned long blinkMillis = 0;
  int blinkInterval = 300;
  bool blinkState = false;

  using Semaforo::yellow;

public:
  Pedestre(int green_pin, int red_pin, int blinkInterval) : blinkInterval(blinkInterval), Semaforo(green_pin,red_pin,red_pin){
    
  };


  void blink() {
    if (millis() - blinkMillis > blinkInterval) {
      blinkMillis = millis();
      blinkState = !blinkState;
      digitalWrite(green_pin, LOW);
      // digitalWrite(yellow_pin, blinkState);
      digitalWrite(red_pin, blinkState);
      
    }
  };


  void setState(char state){
    this->state = state;

    if(state == 'r'){
      green();
    }else if(state == 'g'){
      // digitalWrite(green_pin, HIGH);
      // digitalWrite(red_pin, HIGH);
      red();
    }
  }
};
#endif