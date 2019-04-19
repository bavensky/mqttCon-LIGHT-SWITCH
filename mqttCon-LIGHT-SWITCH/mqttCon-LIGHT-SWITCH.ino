#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttConnector.h>

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"

MqttConnector *mqtt;

int btnUP = 13;
int btnDOWN = 0;
int relayPin = 15;
int relayPinState = HIGH;
boolean btnState = false;
char myName[40];


void init_hardware()
{
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnDOWN, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(relayPin, relayPinState);;
  // serial port initialization
  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
}

void init_wifi() {
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(50);
  const char* ssid =  WIFI_SSID.c_str();
  const char* pass =  WIFI_PASSWORD.c_str();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf ("Connecting to %s:%s\r\n", ssid, pass);
    delay(300);
  }
  Serial.println("WiFi Connected.");
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
  //void sync_advpub(String prefix, String topic, String payload, bool retain);

  if (digitalRead(btnUP) == 0) {
    delay(200);
    btnState = true;
    mqtt->sync_advpub("", "CMMC/PLUG-001/$/command", "ON", false);
//     mqtt->sync_pub("ON");
    while (digitalRead(btnUP) == 0) {
      delay(1);
    }
  } else if (digitalRead(btnDOWN) == 0) {
    delay(200);
    btnState = false;
    mqtt->sync_advpub("", "CMMC/PLUG-001/$/command", "OFF", false);
//     mqtt->sync_pub("OFF");
    while (digitalRead(btnDOWN) == 0) {
      delay(1);
    }
  }

  mqtt->loop();
}
