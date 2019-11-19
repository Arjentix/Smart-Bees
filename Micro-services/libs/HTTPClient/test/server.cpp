/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 18.11.2019
 */

#include "test_runner.h"
#include "HTTPServer.h"
#include <string>
#include <iostream>

using namespace std;

HTTPServer server;

int client_sock;

void recv_big_test()
{
	// Receiving request
	auto request = HTTPServer::get_request(client_sock);
	stringstream ss;
	HTTPHandler::write_request(request, ss);

	string expected_request =
	"POST / HTTP/1.1\r\n"
	"Connection: close\r\n"
	"Content-Length: 1906\r\n"
	"Content-Type: application/json\r\n"
	"\r\n"
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

	ASSERT_EQUAL(ss.str(), expected_request);
}

int main()
{
	server = HTTPServer();
	server.start_server(8881);
	server.turn_to_listen(5);

	cout << "Before connect" << endl;
	client_sock = server.connect_client();

	cout << "Before run_test" << endl;
	TestRunner tr;
	RUN_TEST(tr, recv_big_test);

	HTTPServer::close_con(client_sock);
}
