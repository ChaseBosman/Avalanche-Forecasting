#include <iostream>
#include "RequestHandler.H"
#include "TargetBuilder.h"

int main() {
	TargetBuilder target1("provo");
	std::string targett = target1.get_target();
	RequestHandler handler(targett);
	cout << handler.get_response_body() << '\n';
	return 0;
}