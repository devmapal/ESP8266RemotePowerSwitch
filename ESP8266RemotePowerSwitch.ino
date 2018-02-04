#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <RCSwitch.h>
#include "ui.h"

// Should define wifi_ssid and wifi_password
#include "wifi_config.h"

// Pin the 433 MHz transmitter is connected to
const int TX_PIN = D1;

RCSwitch mySwitch = RCSwitch();
ESP8266WebServer server(80);

bool handleSwitch() {
  if(server.method() != HTTP_PUT) {
    return false;
  }

  const uint8_t BUFFER_SIZE = 10;
  int uri_length = server.uri().length();
  if(uri_length > BUFFER_SIZE || uri_length <= 1) {
    return false;
  }
  
  if(uri_length > BUFFER_SIZE+1 || uri_length <= 1) {
    return false;
  }
  
  char uri[BUFFER_SIZE] = "";
  // Directly remove leading "/"
  strncpy(uri, &server.uri().c_str()[1], BUFFER_SIZE);

  char* groupStr = strtok(uri, "/");
  if(groupStr == NULL) {
    return false;
  }

  char* switchStr = strtok(NULL, "/");
  if(switchStr == NULL) {
    return false;
  }

  int group_no = atoi(groupStr);
  int switch_no = atoi(switchStr);

  if(!server.hasArg(String("state"))) {
    return false;
  }

  bool on = !server.arg(String("state")).compareTo(String("on"));

  if(on == false) {
    mySwitch.switchOff(group_no, switch_no);
  } else {
    mySwitch.switchOn(group_no, switch_no);
  }

  server.send(201);

  return true;
}

void handleNotFound(){
  if(handleSwitch() == true) {
    // Already handled
    return;
  }
  
  server.send(404, "text/plain", "Not found");
}

void handleRoot() {
  server.send(200, "text/html", UI);
}

void setup() {
  Serial.begin(115200);
  
  // Transmitter is connected to TX_PIN
  mySwitch.enableTransmit(TX_PIN);

  WiFi.begin(wifi_ssid, wifi_password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(wifi_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
