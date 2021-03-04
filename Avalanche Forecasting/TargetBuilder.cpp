#include "TargetBuilder.h"
#include <string>


		TargetBuilder::TargetBuilder(std::string location) 
		{
			//target directory for HTTPS request
			target = "/forecast/" + location + "/json";
		}

		std::string TargetBuilder::get_target()
		{
			return target;
		}
