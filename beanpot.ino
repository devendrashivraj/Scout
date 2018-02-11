#include <SparkFun_TB6612.h>
#include <Wire.h>
#include <Servo.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>

#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "/Users/devendra/Documents/Arduino/libraries/Adafruit_BluefruitLE_nRF51/examples/controller/BluefruitConfig.h"

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

extern uint8_t packetbuffer[];

#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 17
#define PWMA 5
#define PWMB 6
#define STBY 16

const int offsetA = -1;
const int offsetB = -1;
int val, reading, sensorEnable = 1;
unsigned long startTime;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
Servo servo1;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
  
  Wire.begin(42);
  Wire.onReceive(receiveCommands);

  servo1.attach(3);
 
  delay(1000);
}

uint8_t buttnum;
boolean pressed;
void loop(){
    uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
    if (len == 0 && reading == 0) return;
  
    // Buttons
    if (packetbuffer[1] == 'B') {
      buttnum = packetbuffer[2] - '0';
      pressed = packetbuffer[3] - '0';
    }
    Serial.println(buttnum);
  
    if (reading == 99 ||
        (buttnum == 5 && pressed)){
      servo1.write(90);
      delay(100);
      startTime = millis();
      while (((millis() - startTime) <= 1300) &&
              (!digitalRead(14) || sensorEnable)&&
              digitalRead(15)){
                motor1.drive(255);
                motor2.drive(150);
                delay(50);
                motor1.brake();
                motor2.brake();
      }    
    }
    if (reading == 108 ||
        (buttnum == 7 && pressed)){
      servo1.write(45);
      delay(100);
      motor1.drive(-255);
      motor2.drive(150);
      delay(750);
      motor1.brake();
      motor2.brake();  
    }
    if (reading == 114 ||
        (buttnum == 8 && pressed)){
      servo1.write(135);
      delay(100);
      motor1.drive(255);
      motor2.drive(-150);
      delay(750);
      motor1.brake();
      motor2.brake();  
    }
    if (buttnum == 6 && pressed){
      servo1.write(90);
      delay(100);
      motor1.drive(-255);
      motor2.drive(-150);
      delay(1300);
      motor1.brake();
      motor2.brake();  
    }
    if (buttnum == 1 && pressed){
      sensorEnable = !sensorEnable;
    }
    reading = 0;
}

void receiveCommands(){
    reading = Wire.read(); 
}

