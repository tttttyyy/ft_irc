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

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


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
