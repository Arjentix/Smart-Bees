/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 03.05.19
*/

#include "ConfigReader.h"
#include "PhotonConfigReader.h"
#include "AliceConnector.h"
#include "MQTTPublisher.h"
#include "TokenHandler.h"
#include "LogPrinter.h"
#include <string>
#include <vector>
#include <future>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/*
* This is the main file of Smart Home Gate program.
* It connects to the Yandex Alice skill and communticates with Photons via MQTT protocol.
* When it receives a token from Alice it sends relevant command to the appropriate MQTT topic
* and sends answer to the Alice.
*/

using namespace std;

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

int main(int argc, char** argv)
{
	// Daemon mode
	if (argc > 1 && strcmp(argv[1], "-d") == 0) {
		int fork_res = fork();
		if (fork_res == -1) {	// Error
			return -1;
		}
		if (fork_res > 0) {		// Parent
			return 0;
		}
		
		// Child 
		// Closing useless fds
		close(0);
		close(1);
		close(2);
	}

	/* Child */
	try {
		LogPrinter::print("!!!---Program started---!!!");

		ConfigReader::reader.set_file_name("config.txt");
		ConfigReader::reader.read_config();

		/* Initialization */

		// Reads config and gets new topic for every Photon
		PhotonConfigReader	photon_conf_reader(
			ConfigReader::reader.read_value_by_key<string>("PHOTON_CONFIG_FILE")
		);

		// Does all communication with Alice
		AliceConnector		alice_conn(
			ConfigReader::reader.read_value_by_key<string>("ACTIVE_CONNECTIONS_SERVICE_IP"),
			ConfigReader::reader.read_value_by_key<int>("ACTIVE_CONNECTIONS_SERVICE_PORT")
		);

		// Does publishing messages to the MQTT topics
		MQTTPublisher		mqtt_pub(
			ConfigReader::reader.read_value_by_key<string>("MQTT_BROKER_IP"),
			ConfigReader::reader.read_value_by_key<int>("MQTT_BROKER_PORT")
		);

		// Checks for existing token and returns relevant parameters
		TokenHandler		tok_hand(
			ConfigReader::reader.read_value_by_key<string>("TOKENS_CONFIG_FILE")
		);

		string				token;
		string				topic;
		string				command;
		bool				res;

		/* Signal capture */
		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);

		/* Initializing Photons */
		for (
			const auto& [photon_mac, photon_new_topic] :
			photon_conf_reader.get_all_configs()
		) {
			/* Sending topics to the Photons for them to subscribe */
			mqtt_pub.publish(photon_mac, photon_new_topic, true);
		}

		vector<future<void>> futures;
		while (!finish) {
			token = alice_conn.get_token();
			// System signal can be received during get_token() function,
			// So finish flag should be checked once more
			if (finish) {
				break;
			}
			if (token == "Check") {
				LogPrinter::print("Alice wants to check us...");
				alice_conn.send_ok();
				LogPrinter::print("Check passed");
			}
			else if (token != "") {
				futures.push_back(
					async([&] () {
						LogPrinter::print("Token getted: '" + token + "'");
						res = tok_hand.find(token, topic, command);
						if (res == true) {
							LogPrinter::print("Topic: \"" + topic + "\"");
							LogPrinter::print("Command: " + command);
							LogPrinter::print("Publishing");
							mqtt_pub.publish(topic, command, false);
							LogPrinter::print("Sending OK");
							alice_conn.send_ok();
						}
						else {
							alice_conn.send_err();
						}
					})
				);
			}
		}
	}
	catch (const std::exception &error) {
		LogPrinter::print(error.what());
	}

	LogPrinter::print("Program finished");
}
