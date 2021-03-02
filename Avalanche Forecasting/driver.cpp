#include <iostream>
#include "RequestHandler.H"

int main() {
	RequestHandler handler;
	cout << handler.get_response_body() << '\n';
	return 0;
}