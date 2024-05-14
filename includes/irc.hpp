#ifndef IRC_HPP
# define IRC_HPP

# include <map>
# include <cctype>
# include <cerrno>
# include <vector>
# include <string>
# include <fstream>
# include <fcntl.h>
# include <sstream>
# include <stdio.h>
# include <cstring>
# include <stdlib.h>
# include <unistd.h>
# include <iostream>
# include <exception>
# include <algorithm>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "CommandHandler.hpp"
# include "Exceptions.hpp"
# include "MessageController.hpp"
# include "ClientManager.hpp"


// --------------- Command Response --------------------------
void	SendMessageToClient(const Client &client, const std::string &message);
void	PartMessage(const Client &client, std::string const &channelName);
void	KickMessage(const Client &client, std::string const &channelName,std::string const &admin);
void	UserModeMessage(const Client &client);
void	ChannelModeMessage(const Client &client, const std::string &channelName);

// ------------IMessenger---------------------------------------------
void	SendMessageWithSocket(int clientSocket,	const std::string &message);

int     custom_stoi(const std::string& str);
std::string to_string(int value);


#endif
