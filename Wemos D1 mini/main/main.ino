/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 30.10.2019
 */

/**
 * This scetch does the next things:
 * 1. Connects to a WiFi network;
 * 2. Finds MQTT broker and connects to it;
 * 3. Subscribes to the topic with it's mac-adress as name;
 * 4. Reads a new topic name to subscribe;
 * 5. Subscribes to the new topic;
 * 6. Listens for the incomming commands;
 * 7. Redirects getted command to the UART output.
 * 8. Rerturns to the step #6.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#ifndef STASSID
#define STASSID "****"
#define STAPSK  "****"
#endif

#define PORT 1883

/*
* callback() - is calling when a message from topic was received.
*/
void callback(char* topic, byte* payload, unsigned int length);

/*
* broker_connect() - connects to the brocker.
*/
void broker_connect();

/*
* mactopic_subcribe() - subscribes to the topic with mac address in name
*/
void mactopic_subscribe();

const char* ssid     = STASSID;
const char* password = STAPSK;
byte broker[] = {192, 168, 0, 19};
char mac_str[17];
WiFiClient espClient;
PubSubClient client(espClient);
//SoftwareSerial s(3, 1);

int first_time = true;

void setup() {
  Serial.begin(9600);

  // Start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  randomSeed(micros());

  client.setCallback(callback);
  broker_connect();

  if (client.connected()) {
    mactopic_subscribe();

    client.loop();
  }
}

void loop() {
  if (client.connected()) {
    client.loop();
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  char log_mes[256];
  char p[length + 1];

  if (first_time) {
    Serial.println("Initial callback started");

    memcpy(p, payload, length);
    p[length] = '\0';

    sprintf(log_mes, "Unsubscribing from %s", mac_str);
    Serial.println(log_mes);
    client.unsubscribe(mac_str);

    sprintf(log_mes, "Subscribing to the \"%s\"", p);
    Serial.println(log_mes);
    client.subscribe(p);

    first_time = false;
  }
  else {
    Serial.println("Callback!");

    memcpy(p, payload, length);
    p[length] = '\0';

    sprintf(log_mes, "Getted %s", p);
    Serial.println(log_mes);
    Serial.println(p);
   // s.println(p);
  }
}

void broker_connect()
{
  char log_mes[256];
//  client.setServer("test.mosquitto.com", PORT);
//  while(!client.connected()) {
//    Serial.println("Trying connect to a broker");
//    client.connect("Beeclient");
//  }

  // Connecting to the broker
  for(;; client.setServer(broker, PORT)) {
    sprintf(log_mes, "Trying %d.%d.%d.%d on port %d...", broker[0], broker[1], broker[2], broker[3], PORT);
    Serial.println(log_mes);
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    client.connect(clientId.c_str());

    if (client.connected()) {
      sprintf(log_mes, "Connected to the broker %d.%d.%d.%d on port %d", broker[0], broker[1], broker[2], broker[3], PORT);
      Serial.println(log_mes);
      break;
    }
    else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
//      if (broker[2] == 255) {
//        sprintf(log_mes, "Can't find any broker on port %d", PORT);
//        Serial.println(log_mes);
//        break;
//      }
//      else if (broker[3] == 255) {
//        broker[3] = 0;
//        broker[2]++;
//      }
//      else {
//        broker[3]++;
//      }
    }
  }
}

void mactopic_subscribe()
{
  byte mac[6];
  char log_mes[256];
  WiFi.macAddress(mac);
  sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  sprintf(log_mes, "Subscribing to the topic \"%s\"", mac_str);
  Serial.println(log_mes);
  client.subscribe(mac_str);
}
