#include "CommandHandler.hpp"
#include "Exceptions.hpp"
#include "Commands.hpp"

CommandHandler	*CommandHandler::instance = NULL;

CommandHandler::CommandHandler()
{
	if (instance)
		this->~CommandHandler();
	else
	{
		instance = this;
		InitilizeCommands();
	}
}

CommandHandler	*CommandHandler::getHandler()
{
	if (!instance)
		new CommandHandler();
	return (instance);
}


CommandHandler::~CommandHandler()
{
	ClearCommands();
}

void	CommandHandler::InitilizeCommands()
{
	commands["PASS"] = &Command::execute_pass;
	commands["NICK"] = &Command::execute_nick;
	commands["USER"] = &Command::execute_user;
	commands["PING"] = &Command::execute_ping;
	commands["PONG"] = &Command::execute_pong;
	commands["PRIVMSG"] = &Command::execute_privmsg;
	commands["NOTICE"] = &Command::execute_notice;
	commands["JOIN"] = &Command::execute_join;
	commands["PART"] = &Command::execute_part;
	commands["KICK"] = &Command::execute_kick;
	commands["QUIT"] = &Command::execute_quit;
	commands["MODE"] = &Command::execute_mode;
	commands["WHO"] = &Command::execute_who;
	commands["BOT"] = &Command::execute_bot;
	commands["BOTME"] = &Command::execute_botme;
	commands["FTP"] = &Command::execute_ftp;
}

void	CommandHandler::ClearCommands()
{
		delete instance;
}

void	CommandHandler::ExecuteCommand(Client &sender, const CommandData &data)
{
	it = commands.find(data.command);
	if (it != commands.end())
	{
		commands[data.command](sender, data.args);
		// Command::getCommand()->execute(sender, data.args, it->second);
	}
	else
	{
		throw IRCException(sender.getNick(), " " + data.command + " :Unknown command", 421);//UnknownCommand(sender.getNick(), data.command);
	}
}
