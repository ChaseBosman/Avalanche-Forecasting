#include "Response.h"

Response::Response(std::string new_response, std::string new_location)
{
	response = new_response;
	location = new_location;
}

std::string Response::get_response()
{
	return response;
}

std::string Response::get_location()
{
	return location;
}