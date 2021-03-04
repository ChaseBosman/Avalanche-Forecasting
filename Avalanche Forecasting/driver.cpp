#include <iostream>
#include "RequestHandler.h"
#include "TargetBuilder.h"
#include "Response.h"

int main() {
	bool complete = false;

	while (!complete)
	{
		TargetBuilder target1("provo");
		std::string target = target1.get_target();
		RequestHandler handler(target);
		Response provo(handler.get_response_body(), "provo");
		cout << provo.get_response() << '\n';
		complete = true;
	}
	
	return 0;
}