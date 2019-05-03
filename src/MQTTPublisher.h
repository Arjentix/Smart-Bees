/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 02.0.19
*/

#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

/*
* This file describes MQTTPublisher class, which can publish messages in the given topic.
* It connects to the broker on host and port provided in conctructor.
*/

#include <mosquitto.h>
#include <string>

class MQTTPublisher {
private:
	std::string		_host;
	int			_port;
	struct mosquitto	*_mosq; 	// Mosquitto instance for API

public:
	/*
	* MQTTPublisher() - gets broker's host and port (usually 1883) and connects to it.
	* Can throw std::runtime_error if can't create new mosquitto instance or can't connect to server.
	*/
	MQTTPublisher(std::string host, int port);

	~MQTTPublisher();

	/*
	* publish() - publish given message to the given topic with quality = 1 and not retain.
	* Cat thow std::runtime_error if some error occured in publishing process.
	*/
	void publish(std::string topic, std::string mes);
};

#endif // MQTT_PUBLISHER_H
