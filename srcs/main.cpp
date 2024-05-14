#include "irc.hpp"
//write max client fd
//check in while loop whether client fd is max if yes send limit message to new socket/client and close the socket
//master socket?
//listen, max queue pending connecton recommended size(2 arg)

int	ValidateInput(int argc, std::string stringPort)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments!!" << std::endl // avelacnel guynery
			<< "Try like: ./irc <port> <password>" << std::endl; // avelacnel guynery
		exit(EXIT_FAILURE);
	}
	int port = custom_stoi(stringPort);
	if (port < 0 || port > 65535 || stringPort.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Wrong PORT specified!!! Please input a valid PORT" << std::endl // avelacnel guynery
		<< "It should be a number between 0 and 65535" << std::endl; // avelacnel guynery
		exit(EXIT_FAILURE);
	}
	return(port);
}

int main(int argc, char **argv)
{
	Server	server(ValidateInput(argc, argv[1]), argv[2]);

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



