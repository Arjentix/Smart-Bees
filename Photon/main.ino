// This #include statement was automatically added by the Particle IDE.
#include "MQTT.h"

#define PORT 1883

/*
* callback() - is calling when a message from topic was received.
*/
void callback(char* topic, byte* payload, unsigned int length);

/*
* broker_connect() - connects to the brocker with minimal local ip address.
*/
void broker_connect();

/*
* mactopic_subcribe() - subscribes to the topic with mac address in name
*/
void mactopic_subscribe();

byte broker[] = { 192, 168, 43, 33 };
char mac_str[17];
MQTT client = MQTT(broker, PORT, callback);
int LED = D1;
int first_time = true;

void setup()
{
	Serial.println("Hello, I'm stupid Photon!");

	pinMode(LED, OUTPUT);

	broker_connect();

	if (client.isConnected()) {
		mactopic_subscribe();

		client.loop();
	}
}

void loop()
{
	if (client.isConnected()) {
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
		if (digitalRead(LED) == LOW) {
			digitalWrite(LED, HIGH);
		}
		else {
			digitalWrite(LED, LOW);
		}

		delay(1000);
	}
}

void broker_connect()
{
	char log_mes[256];

	// Connecting to the broker
	for(;; client.setBroker(broker, PORT)) {
		sprintf(log_mes, "Trying %d.%d.%d.%d on port %d...", broker[0], broker[1], broker[2], broker[3], PORT);
		Serial.println(log_mes);

		client.connect("photonclient");

		if (client.isConnected()) {
			sprintf(log_mes, "Connected to the broker %d.%d.%d.%d on port %d", broker[0], broker[1], broker[2], broker[3], PORT);
			Serial.println(log_mes);
			break;
		}
		else {
			if (broker[2] == 255) {
				sprintf(log_mes, "Can't find any broker on port %d", PORT);
				Serial.println(log_mes);
				break;
			}
			else if (broker[3] == 255) {
				broker[3] = 0;
				broker[2]++;
			}
			else {
				broker[3]++;
			}
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
