#include "Arduino.h"
#include "BluetoothSerial.h"
#include "esp32-hal-gpio.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 
#endif

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32Test");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  char input;

  if(SerialBT.available()){
    input = SerialBT.read();
    if(input == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (input == '2') {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  delay(100);
}
