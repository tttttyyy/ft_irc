#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include "Commands.hpp"
# include "CommandData.hpp"
# include <map>
class Command;

class CommandHandler
{
	private: //for singleton
		static CommandHandler *instance;
	private:
		typedef void (*CommandFunction)(Client&, const std::vector<std::string>&);

    	std::map<std::string, CommandFunction> commands;
		std::map<std::string, CommandFunction>::iterator it;
		void	InitilizeCommands();
		void	ClearCommands();

	public:
		CommandHandler();
		~CommandHandler();

		void	ExecuteCommand(Client &client, const CommandData &data);
		static	CommandHandler *getHandler();
};

#endif // COMMAND_HANDLER_HPP