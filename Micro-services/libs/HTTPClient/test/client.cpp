/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 18.11.2019
 */

#include "HTTPClient.h"
#include <iostream>

using namespace std;

int main()
{
	HTTPClient client;

	cout << "Before connect" << endl;
	client.connect_to_server("127.0.0.1", 8881);

	// Sedning request
	string body =
	"{\n"
		"\"watetfgqewilhrueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafghaidugoaighfdga87ygoqfhgd\","
		"\"tttttttttttttttttttueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafyyyyyyyyyyyyyyyyyyyyy\","
		"\"watetfgqewilhrueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafghaidugoaighfdga87ygoqfhgd\","
		"\"tttttttttttttttttttueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafyyyyyyyyyyyyyyyyyyyyy\","
		"\"watetfgqewilhrueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafghaidugoaighfdga87ygoqfhgd\","
		"\"tttttttttttttttttttueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafyyyyyyyyyyyyyyyyyyyyy\","
		"\"watetfgqewilhrueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafghaidugoaighfdga87ygoqfhgd\","
		"\"tttttttttttttttttttueufhdlfhasjdflsdfhdsoufhqjdflaskdfghlasdhgjdsalghsddasfhlsdfprqihugldsfkhgprqihutkdsaltqrihutdalktidtuhqoriheriu\" : \"qhuidfhgiljdfkghadfglafkhgfdpgiqdshdshgdflahgldghadslghdghiugrepghrguhfdugyafyyyyyyyyyyyyyyyyyyyyy\""
	"}";

	cout << "Before send" << endl;
	client.send_request({
		HTTPHandler::Method::POST, "/",
		{},
		{
			{"Connection", "close"},
			{"Content-Length", to_string(body.size())},
			{"Content-Type", "application/json"}
		},
		body
	});

	cout << "Finishing" << endl;
	client.close_conn();
	return 0;
}
