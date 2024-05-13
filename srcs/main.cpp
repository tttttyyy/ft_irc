#include "irc.hpp"

void	ValidateInput(int argc, std::string stringPort)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments!!" << std::endl // avelacnel guynery
			<< "Try like: ./irc <port> <password>" << std::endl; // avelacnel guynery
		exit(EXIT_FAILURE);
	}
	int port = stoi(stringPort);
	if (port < 0 || port > 65535 || stringPort.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Wrong PORT specified!!! Please input a valid PORT" << std::endl // avelacnel guynery
		<< "It should be a number between 0 and 65535" << std::endl; // avelacnel guynery
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	ValidateInput(argc, argv[1]);

	Server	server(atoi(argv[1]), argv[2]);

	server.Setup();

	while(true)
	{
		server.ResetSockets();
		server.WaitForActivity();
		server.HandleIncomingConnections();
		server.ListenForClientInput();
	}
	return (0);
}



