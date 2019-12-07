/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 05.05.19
*/

#ifndef PHOTON_CONFIG_READER_H
#define PHOTON_CONFIG_READER_H

/*
* This file describes PhotonConfigReader class.
* It reads config file and provide mac address and MQTT topic,
* which must be subscribed by specific Photon.
*/

#include <string>
#include <fstream>
#include <vector>
#include <utility>

class PhotonConfigReader {
private:
	std::ifstream _config_file;

public:
	/*
	* PhotonConfigReader() - opens photon's config file with path config_path.
	* Can throw std::runtime_error if can't open file.
	*/
	PhotonConfigReader(const std::string& config_path);

	~PhotonConfigReader();

	/*
	* get_all_configs() - reads all line in config file and returns std::vector
	* of pair with photon mac and new photon topic.
	*/

	std::vector<std::pair<std::string, std::string>> get_all_configs();

	/*
	* get_next() - reads config file. Places getted photon mac addres in photon_mac
	* and getted photon topic in photon_new_topic and returns true,
	* if reading was successfull or returns false, if every lines was readed.
	*/
	bool get_next(std::string &photon_mac, std::string &photon_new_topic);
};

#endif // PHOTON_CONFIG_READER_H
