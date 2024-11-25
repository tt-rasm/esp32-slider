#include "Arduino.h"
#include "BluetoothSerial.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal-matrix.h"
#include "esp32-hal.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 
#endif

#define EN_PIN 18
#define STEP_PIN 19
#define DIR_PIN 21

BluetoothSerial SerialBT;

int noOfSteps = 250;
int microSecondsDelay = 1000;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32Test");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(DIR_PIN, LOW);
  digitalWrite(EN_PIN, LOW);
}

void rotateMotor(int steps, bool direction, int speed) {
  digitalWrite(DIR_PIN, direction);

  for(int i = 0; i < steps; i++){
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP_PIN,LOW);
    delayMicroseconds(speed);
  }
}

void loop()
{
  char input;

  if(SerialBT.available()){
    input = SerialBT.read();
    if(input == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
      rotateMotor(1600, 1, 2000);
      digitalWrite(LED_BUILTIN, LOW);
    } else if (input == '2') {
      digitalWrite(LED_BUILTIN, HIGH);
      rotateMotor(1600, 0, 2000);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  delay(100);

}
