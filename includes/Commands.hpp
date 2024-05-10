#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <map>
# include <vector>
# include "Client.hpp"
# include <iostream>
# include <string>
# include "Exceptions.hpp"
# include "Server.hpp"
# include "MessageController.hpp"
# include "ClientManager.hpp"




struct CommandType
{
	enum Type
	{
		pass,
		nick,
		user,
		ping,
		pong,
		privmsg,
		notice,
		join,
		part,
		kick,
		quit,
		mode,
		who,
		cap,
		ls,
		bot,
		botme,
		ftp
	};
};

// class ICommand;
class Command
{
	public:
	Command();
	~Command();
	static	Command *instance;
	static	Command *getCommand();
	    // Define function pointer type for execute and validate functions
    typedef void(*CommandFunction)(Client &sender, const std::vector<std::string> &arguments);

    // Array of function pointers for execute and validate
    CommandFunction execute[sizeof(CommandType::Type)];
    CommandFunction validate[sizeof(CommandType::Type)];
	// void	execute[sizeof(CommandType::Type)](Client &sender, const std::vector<std::string> &arguments);
	// void	validate[sizeof(CommandType::Type)](Client &sender, const std::vector<std::string> &arguments);

};


#endif // COMMANDS_HPP