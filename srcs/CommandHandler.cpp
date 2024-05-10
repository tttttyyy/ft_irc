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
	commands[CommandType::Type pass] = Command::getCommand();
	commands[CommandType::Type nick] = Command::getCommand();
	commands[CommandType::Type user] = Command::getCommand();
	commands[CommandType::Type ping] = Command::getCommand();
	commands[CommandType::Type pong] = Command::getCommand();
	commands[CommandType::Type privmsg] = Command::getCommand();
	commands[CommandType::Type notice] = Command::getCommand();
	commands[CommandType::Type join] = Command::getCommand();
	commands[CommandType::Type part] = Command::getCommand();
	commands[CommandType::Type kick] = Command::getCommand();
	commands[CommandType::Type quit] = Command::getCommand();
	commands[CommandType::Type mode] = Command::getCommand();
	commands[CommandType::Type who] = Command::getCommand();
	commands[CommandType::Type cap] = Command::getCommand();
	commands[CommandType::Type bot] = Command::getCommand();
	commands[CommandType::Type botme] = Command::getCommand();
	commands[CommandType::Type ftp] = Command::getCommand();
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
		throw UnknownCommand(sender.getNick(), data.command);
	}
}
