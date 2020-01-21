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
#define STASSID "******"
#define STAPSK  "******"
#endif

#define PORT 18830
#define DEBUG 0

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

/*
 * print() - prints given string if DEBUG is set to 1
 */
void print(String str);

void blink();

const char* ssid     = STASSID;
const char* password = STAPSK;
int debug_flag = DEBUG;
byte broker[] = {192, 168, 43, 33};
char mac_str[17];
WiFiClient espClient;
PubSubClient client(espClient);
int first_time = true;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  char log_mes[256];
  Serial.begin(9600);

  // Start by connecting to a WiFi network
  print("");
  print("");
  sprintf(log_mes, "Connecting to %s", ssid);
  print(log_mes);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    print(".");
  }

  print("");
  print("WiFi connected");
  print("IP address: ");
  memcpy(log_mes, WiFi.localIP(), 256);
  print(log_mes);

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
  blink();
  char log_mes[256];
  char p[length + 1];

  if (first_time) {
    print("Initial callback started");

    memcpy(p, payload, length);
    p[length] = '\0';

    sprintf(log_mes, "Unsubscribing from %s", mac_str);
    print(log_mes);
    client.unsubscribe(mac_str);

    sprintf(log_mes, "Subscribing to the \"%s\"", p);
    print(log_mes);
    client.subscribe(p);

    first_time = false;
  }
  else {
    print("Callback!");

    memcpy(p, payload, length);
    p[length] = '\0';

    sprintf(log_mes, "Getted %s", p);
    print(log_mes);
    Serial.print(p);
  }
}

void broker_connect()
{
  char log_mes[256];

  // Connecting to the broker
  for(;; client.setServer(broker, PORT)) {
    sprintf(log_mes, "Trying %d.%d.%d.%d on port %d...", broker[0], broker[1], broker[2], broker[3], PORT);
    print(log_mes);
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    client.connect(clientId.c_str());

    if (client.connected()) {
      sprintf(log_mes, "Connected to the broker %d.%d.%d.%d on port %d", broker[0], broker[1], broker[2], broker[3], PORT);
      print(log_mes);
      break;
    }
    else {
      print("failed, rc=" + client.state());
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
  print(log_mes);
  client.subscribe(mac_str);
}

void print(String str)
{
  if (debug_flag == 1) {
    Serial.println(str);
  }
}

void blink()
{
  for (int i = 0; i < 2; ++i) {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(100);
    digitalWrite(BUILTIN_LED, LOW);
    delay(100);
  }
}
