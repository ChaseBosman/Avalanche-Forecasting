#include <iostream>
#include "RequestHandler.h"
#include "TargetBuilder.h"
#include "Response.h"

int main() 
{
	std::map<std::string, std::unique_ptr<Response>> response_map;

	while (true)
	{
		cout << "Enter a Utah Avalanche Center location to search. Type 'help' for help, 'exit' to leave the program: ";
		std::string user_input;
		cin >> user_input;
		if (user_input == "exit") 
			return EXIT_SUCCESS;
		if(user_input == "help")
		{
			cout << "Locations include: Logan, Ogden, Uintas, Salt-Lake, Provo, Skyline, Moab, and Abajos.\n";
		}
		// if the response isn't already cached
		else if (response_map.find(user_input) == response_map.end())
		{
			// build the target path with the user input and save it to target
			TargetBuilder curr__target(user_input);
			std::string target = curr__target.get_target();

			// handle the get requested target
			RequestHandler handler(target);

			// map the response of the user entered location into a map containing response objects
			response_map.insert(std::make_pair(user_input, new Response(handler.get_response_body(), user_input)));

			// print response
			cout << response_map[user_input]->get_response() << '\n';
		}
		else 
		{
			// location has already been requested and mapped
			cout << response_map[user_input]->get_response() << '\n';
		}
		cout << '\n';
	}
	
	return 0;
}