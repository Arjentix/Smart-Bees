/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 02.0.19
*/

#include <mosquitto.h>
#include <string>

class MQTTPublisher {
private:
	std::string		_host;
	int			_port;
	struct mosquitto	*_mosq;

public:
	MQTTPublisher(std::string host, int port);
	~MQTTPublisher();

	void publish(std::string topic, std::string mes);
};
