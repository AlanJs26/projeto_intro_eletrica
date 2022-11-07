#ifndef SEMAFORO_h
#define SEMAFORO_h

class Semaforo
{
protected:
  int green_pin;
  int yellow_pin;
  int red_pin;
  char state = 'r';

public:
  Semaforo(int green_pin, int yellow_pin, int red_pin) : green_pin(green_pin), yellow_pin(yellow_pin), red_pin(red_pin){
    pinMode(green_pin, OUTPUT);
    pinMode(yellow_pin, OUTPUT);
    pinMode(red_pin, OUTPUT);
  };

  void off(){
    digitalWrite(green_pin, LOW);
    digitalWrite(yellow_pin, LOW);
    digitalWrite(red_pin, LOW);
    state = 'y';
  }

  void green()
  {
    digitalWrite(green_pin, HIGH);
    digitalWrite(yellow_pin, LOW);
    digitalWrite(red_pin, LOW);
    state = 'g';
  };

  void yellow()
  {
    digitalWrite(green_pin, LOW);
    digitalWrite(yellow_pin, HIGH);
    digitalWrite(red_pin, LOW);
    state = 'y';
  };

  void red()
  {
    digitalWrite(green_pin, LOW);
    digitalWrite(yellow_pin, LOW);
    digitalWrite(red_pin, HIGH);
    state = 'r';
  };

  virtual void setState(char state){

    if(state == 'r'){
      red();
    }else if(state == 'y'){
      yellow();
    }else{
      green();
    }
    this->state = state;
  }

  char getState()
  {
    return state;
  }

};
#endif
