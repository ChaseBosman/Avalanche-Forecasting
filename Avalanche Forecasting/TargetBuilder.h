#pragma once
#include <string>

class TargetBuilder
{
	std::string target;

	public:
		TargetBuilder(std::string location);
		std::string get_target();
};