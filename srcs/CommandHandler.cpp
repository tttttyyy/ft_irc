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
	
	commands[CommandType::pass] = Command::getCommand();
	commands[CommandType::nick] = Command::getCommand();
	commands[CommandType::user] = Command::getCommand();
	commands[CommandType::ping] = Command::getCommand();
	commands[CommandType::pong] = Command::getCommand();
	commands[CommandType::privmsg] = Command::getCommand();
	commands[CommandType::notice] = Command::getCommand();
	commands[CommandType::join] = Command::getCommand();
	commands[CommandType::part] = Command::getCommand();
	commands[CommandType::kick] = Command::getCommand();
	commands[CommandType::quit] = Command::getCommand();
	commands[CommandType::mode] = Command::getCommand();
	commands[CommandType::who] = Command::getCommand();
	commands[CommandType::cap] = Command::getCommand();
	commands[CommandType::bot] = Command::getCommand();
	commands[CommandType::botme] = Command::getCommand();
	commands[CommandType::ftp] = Command::getCommand();
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
		it->second->validate[it->first](sender, data.args);
		it->second->execute[it->first](sender, data.args);
	}
	else
	{
		throw IRCException(sender.getNick(), " " + data.command + " :Unknown command", 421);//UnknownCommand(sender.getNick(), data.command);
	}
}
