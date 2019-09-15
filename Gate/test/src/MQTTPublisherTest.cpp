#include "MQTTPublisher.h"
#include <exception>
#include <iostream>

int main()
{
	try {
		MQTTPublisher publisher("localhost", 1883);
		publisher.publish("smart_home/lamp", "Command 1", false);
	}
	catch (const std::exception &error) {
		std::cout << error.what() << std::endl;
		return -1;
	}

	return 0;
}
