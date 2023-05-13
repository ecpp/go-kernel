#pragma once
#include <iostream>
#include <string>
#define WIN32_LEAN_AND_MEAN
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../utils/httplib.h"


class API {
private:
	httplib::SSLClient client;
public:
	API() : client("91.191.173.36", 8000) {
		client.enable_server_certificate_verification(false);
	}

	bool signup(std::string username, std::string password, std::string email, std::string hwid);
	int login(std::string username, std::string password, std::string hwid);
	bool testApi();
	bool getUserInfo();
	bool validateProduct(int id);
	bool checkLoaderVersion();
	bool downloadFile(int id, std::string productName);
};