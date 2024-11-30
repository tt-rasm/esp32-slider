#include "Arduino.h"
#include "BluetoothSerial.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal-matrix.h"
#include "esp32-hal.h"
#include <cstring>

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

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
#define STEPS 1600

int speed;
int direction;

boolean newData = false;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32BTDev");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(DIR_PIN, LOW);
  digitalWrite(EN_PIN, LOW);
}

void rotateMotor(bool direction, int speed) {
  digitalWrite(DIR_PIN, direction);

  Serial.println("Motor Start");
  for(int i = 0; i < STEPS * 10.3 ; i++){
    digitalWrite(STEP_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP_PIN,LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.println("Motor Stopped");
}

void readData(){
  static boolean rxInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while(SerialBT.available() > 0 && newData == false) {
    rc = SerialBT.read();

    if(rxInProgress == true) {
      if(rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if(ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';
        rxInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      rxInProgress = true;
    }
  }
}

void parseData() {
  char * strtokIndex;

  strtokIndex = strtok(tempChars,",");
  direction = atoi(strtokIndex);
  strtokIndex = strtok(NULL, ",");
  speed = atoi(strtokIndex);
  Serial.println("Data Parsed");
}

void loop()
{
  readData();

  if(newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    rotateMotor(direction, speed);
    newData = false;
  }
}
