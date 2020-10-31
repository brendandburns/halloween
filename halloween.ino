#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>

#include "secrets.h"

#define DEBUG 0
// #define DEBUG 1

int inputPin1 = 16;
int inputPin2 = 5;
int pirState = LOW;
int detections = 0;

AsyncWebServer server(80);

void debug_print(const char* msg) {
  if (DEBUG) {
    Serial.print(msg);
  }
}

void debug_println(const char* msg) {
  if (DEBUG) {
    Serial.println(msg);
  }
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void getStatus(AsyncWebServerRequest *request)
{
  char response[128];
  snprintf(response, 128, "{ \"state\": %d, \"detections\": %d }", pirState, detections);
  request->send(200, "text/plain", response);
}

void setup()
{
  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);

  Serial.begin(9600);
  debug_println("Monitoring started.");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  debug_print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    debug_print(".");
  }
  debug_println("");

  debug_print("Connected, IP address: ");
  debug_println(WiFi.localIP().toString().c_str());

  server.on("/", HTTP_GET, getStatus);
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
  int val1 = digitalRead(inputPin1);
  int val2 = digitalRead(inputPin2);

  if (val1 == HIGH || val2 == HIGH)
  {
    if (pirState == LOW)
    {
      detections++;
      debug_println((String("Motion detected! ") + detections).c_str());
      pirState = HIGH;
    }
  }
  else
  {
    if (pirState == HIGH)
    {
      debug_println("Motion ended!");
      pirState = LOW;
    }
  }
}