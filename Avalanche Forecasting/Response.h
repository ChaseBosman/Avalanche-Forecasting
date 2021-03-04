#pragma once
#include <string>

class Response
{
	std::string response;
	std::string location;

	public:
		Response(std::string response, std::string location);
		std::string get_response();
		std::string get_location();
};

