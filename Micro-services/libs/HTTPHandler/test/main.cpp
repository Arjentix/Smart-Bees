/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 14.09.2019
*/

#include "HTTPHandler.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

ostream& operator<<(ostream& os, HTTPHandler::Method method) {
	os << static_cast<int>(method);
	return os;
}

void test_get()
{
	istringstream input(
		"GET /wiki/page HTTP/1.1\n"
		"Host: ru.wikipedia.org\n"
		"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\n"
		"Accept: text/html\n"
		"Connection: close\n"
		"\n"
	);
	map<string, string> expected_headers = {
		{"Host", "ru.wikipedia.org"},
		{"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
		{"Accept", "text/html"},
		{"Connection", "close"}
	};

	auto req = HTTPHandler::parse_request(input.str());
	ASSERT_EQUAL(req.method, HTTPHandler::Method::GET);
	ASSERT_EQUAL(req.headers, expected_headers);
	ASSERT_EQUAL(req.uri, "/wiki/page");
	ASSERT(req.body.empty());
}

void test_post()
{
	istringstream input("POST http://www.site.ru/news.html HTTP/1.0\n"
		"Host: www.site.ru\n"
		"Referer: http://www.site.ru/index.html\n"
		"Cookie: income=1\n"
		"Content-Type: application/x-www-form-urlencoded\n"
		"Content-Length: 35\n"
		"\n"
		"login=Petya%20Vasechkin&password=qq\n"
	);

	map<string, string> expected_headers = {
		{"Host", "www.site.ru"},
		{"Referer", "http://www.site.ru/index.html"},
		{"Cookie", "income=1"},
		{"Content-Type", "application/x-www-form-urlencoded"},
		{"Content-Length", "35"},
	};

	map<string, string> expected_body = {
		{"login", "Petya%20Vasechkin"},
		{"password", "qq"}
	};

	auto req = HTTPHandler::parse_request(input.str());
	ASSERT_EQUAL(req.method, HTTPHandler::Method::POST);
	ASSERT_EQUAL(req.headers, expected_headers);
	ASSERT_EQUAL(req.uri, "http://www.site.ru/news.html");
	ASSERT_EQUAL(req.body, expected_body);
}

void test_answer()
{
	HTTPHandler::Answer answer = {
		200,
		"OK",
		{
			{"Content-Type", "text/html"},
			{"Connection", "close"},
		},
		"<html>Hello</html>"
	};
	ostringstream result;
	string expected(
		"HTTP/1.1 200 OK\n"
		"Content-Type: text/html\n"
		"Connection: close\n"
		"\n"
		"<html>Hello</html>"
	);

	HTTPHandler::write_answer(answer, result);
	ASSERT_EQUAL(result.str(), expected);
}

int main()
{
	TestRunner tr;
	RUN_TEST(tr, test_get);
	RUN_TEST(tr, test_post);
	RUN_TEST(tr, test_answer);

	return 0;
}
