/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 02.0.19
*/

#include "MQTTPublisher.h"
#include <stdexcept>
#include <cstring>
#include <errno.h>

#define KEEP_ALIVE 120

MQTTPublisher::MQTTPublisher(std::string host, int port) : _host(host), _port(port)
{
	int res;

	mosquitto_lib_init();

	_mosq = mosquitto_new(NULL, true, NULL);
	if (_mosq == NULL) {
		throw std::runtime_error("MQTT client instance establishing failed: " +  std::string(strerror(res)));
	}

	res = mosquitto_connect(_mosq, _host.c_str(), _port, KEEP_ALIVE);
	if (res != MOSQ_ERR_SUCCESS) {
		switch (res) {
		case MOSQ_ERR_INVAL:
			throw std::runtime_error("MQTT server connection failed cause of invalid parameters");
			break;
		case MOSQ_ERR_ERRNO:
			throw std::runtime_error("MQTT server connection failed: " +  std::string(strerror(errno)));
			break;
		default:
			throw std::runtime_error("MQTT server connection failed cause of unknown error: " +  std::string(strerror(errno)));
		}
	}
}

void MQTTPublisher::publish(std::string topic, std::string mes)
{
	mosquitto_publish(_mosq, NULL, topic.c_str(), mes.length(), mes.c_str(), 1, false);
}

MQTTPublisher::~MQTTPublisher()
{
	mosquitto_destroy(_mosq);
	mosquitto_lib_cleanup();
}
