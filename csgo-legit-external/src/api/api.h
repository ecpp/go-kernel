#pragma once
#include <iostream>
#include <string>
#define WIN32_LEAN_AND_MEAN
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../utils/httplib.h"
#include "../utils/obfuscate.h"


class API {
private:
	httplib::SSLClient client;
public:
	API() : client("91.191.173.36", 8000) {
		client.enable_server_certificate_verification(false);
	}

	bool validateProduct(int id);
	bool checkVersion(std::string productName);
	bool downloadDriver(int id, int type);
	std::string downloadMapper();
};