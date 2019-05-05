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
* signal_handler() - handles SIGINT and SIGTERM signals from system.
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
		PhotonConfigReader	photon_conf_reader("photon.conf");	// Reads config and gets new topic for every Photon
		AliceConnector		alice_conn();				// Does all comunication with Alice
		MQTTPublisher		mqtt_pub("localhost", 1883);		// Does publishing messages to the MQTT topis
		TokenHandler		tok_hand("token.base");			// Checks for existing token and returns relevant parameters
		std::string		photon_mac;
		std::string		photon_new_topic;
		std::string		token;
		std::string		topic;
		std::string		command;
		bool			res;

		/* Signal capture */
		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);

		/* Initializing Photons */
		res = photon_conf_reader.get_next(photon_mac, photon_new_topic);
		while (res != false) {
			/* Sending topics to the Photons for them to subscribe */
			mqtt_pub.publish(photon_mac, photon_topic);
			res = photon_conf_reader.get_next(photon_mac, photon_topic);
		}

		while (!finish) {
			token = alice_conn.get_token();
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
