#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "Semaforo.h"
#include "Cruzamento.h"
#include "Via.h"
#include "Pedestre.h"

int scanTime = 1;  //In seconds
BLEScan* pBLEScan;

Semaforo* s1 = new Semaforo(27,15,14);
Semaforo* s2 = new Semaforo(19,18,25);
Pedestre* p1 = new Pedestre(12,13,200);

Cruzamento* cruzamento = new Cruzamento(3000, 1500, 2);

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    if (advertisedDevice.getName() != "") {
      if (advertisedDevice.getName() == "Mi Smart Band 5") {
      //Serial.printf("Advertised Device: %s", advertisedDevice.toString().c_str());
      // Serial.print(" RSSI: ");
      // Serial.println(advertisedDevice.getRSSI());

      
        if (advertisedDevice.getRSSI() > -60) {
          cruzamento->setEstagioForced(1);
          digitalWrite(5, HIGH);
        } else {
          digitalWrite(5, LOW);
        }
      }
    }
  }
};


void StartScan(BLEScanResults results){
  pBLEScan->start(3, StartScan);
}


void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);

  cruzamento->forcedFraction = 30;

  Via* via1 = new Via(1);
  via1->attachSemaforo(s1);

  Via* via2 = new Via(2);
  via2->configure(4000);
  via2->attachSemaforo(s2);
  via2->attachPedestre(p1);
  
  cruzamento->configureEstagio(0, 15000, 2);
  cruzamento->attachVia(0, via1, 'g');
  cruzamento->attachVia(0, via2, 'r');
  

  cruzamento->configureEstagio(1, 10500, 2);
  cruzamento->attachVia(1, via1, 'r');
  cruzamento->attachVia(1, via2, 'g');

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(100);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(3, StartScan, true);

// s1->green();
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

  // BLEScanResults foundDevices = pBLEScan->start(scanTime);
  // Serial.print("Devices found: ");
  // Serial.println(foundDevices.getCount());
  // //Serial.println("Scan done!");
  // pBLEScan->clearResults();
  
  //p1->blink();
  // delay(1000);
}
