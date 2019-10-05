/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 05.05.19
*/

#include "PhotonConfigReader.h"
#include <stdexcept>
#include <cstring>
#include <memory>

/*
* This file defines PhotonConfigReader class, which is decribed in the PhotonConfigReader.h.
* See all methods documentation in the header file
*/

PhotonConfigReader::PhotonConfigReader(const std::string& config_path)
	: _config_file(std::ifstream(config_path))
{
	if (!_config_file.is_open()) {
		throw std::runtime_error("Can't open photon's config file");
	}
}

PhotonConfigReader::~PhotonConfigReader()
{
	_config_file.close();
}

std::vector<std::pair<std::string, std::string>> PhotonConfigReader::get_all_configs()
{
	std::vector<std::pair<std::string, std::string>> res;
	std::string mac, topic;

	while(get_next(mac, topic)) {
		res.push_back(std::move(std::make_pair(mac, topic)));
	}

	return res;
}

bool PhotonConfigReader::get_next(
	std::string &photon_mac, std::string &photon_new_topic
)
{
	char line[256];
	char getted_mac[128];
	char getted_topic[128];
	int len;

	if (_config_file.getline(line, 256)) {
		sscanf(line, "%[^=] = %[^\n]", getted_mac, getted_topic);
		len = strlen(getted_mac);
		if (getted_mac[len - 1] == ' ') {
			getted_mac[len - 1] = '\0';
		}

		photon_mac = getted_mac;
		photon_new_topic = getted_topic;
		return true;
	}

	return false;
}
