/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 03.05.19
*/

#include "AliceConnector.h"
#include "MQTTPublisher.h"
#include "TokenHandler.h"
#include "LogPrinter.h"
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/*
* This is the main file of Smart Home Gate program.
* It connects to the Yandex Alice skill and communticates with Photons via MQTT protocol.
* When it receives a token from Alice it sends relevant command to the appropriate MQTT topic
* and sends answer to the Alice.
*/

/*
* finish - bool variable that indicates should program stop working or not.
* It is regulared only by signal_hanlder (see below).
*/
bool finish = false;

/*
* signal_handler() - handles SIGIGT and SIGTERM signals from system.
*/
void signal_handler(int sig)
{
	LogPrinter::print("Stoped by system signal");
	finish = true;
}

int main()
{
	int fork_res;

	fork_res = fork();
	if (fork_res == -1) {	// Error
		return -1;
	}
	if (fork_res != 0) {	// Parent
		return 0;
	}

	/* Child */
	try {
		/* Closing useless fds */
		close(0);
		close(1);
		close(2);

		/* Initialization */
		AliceConnector		alice_conn();
		MQTTPublisher		mqtt_pub();
		TokenHandler		tok_hand();
		std::string		token;
		std::string		topic;
		std::string		command;
		bool			res;

		while (!finish) {
			token = alice_conn.get_tokents();
			res = tok_hand.find(token, topic, command);
			if (res == true) {
				mqtt_pub.publish(topic, command);
				alice_conn.send_ok();
			}
			else {
				alice_conn.send_err();
			}
		}
	}
	catch (const std::exception &error) {
		LogPrinter::print(error.what());
	}
}
