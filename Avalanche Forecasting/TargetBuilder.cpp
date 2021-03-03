#include "TargetBuilder.h"
#include <string>


		TargetBuilder::TargetBuilder(std::string location) 
		{
			target = "/forecast/" + location + "/json";
		}

		std::string TargetBuilder::get_target()
		{
			return target;
		}
