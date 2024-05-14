#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include "irc.hpp"

class Client;

struct CommandData
{
	std::string command;
	std::vector<std::string> args;
};

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
		invite,
		topic,
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

class CommandHandler
{
	private:
		static CommandHandler *instance;
		typedef void (*CommandFunction)(Client&, const std::vector<std::string>&);
    	std::map<std::string, CommandFunction> commands;
		void	InitilizeCommands();

	public:
		CommandHandler();
		~CommandHandler();

		void	ExecuteCommand(Client &client, const CommandData &data);
		static	CommandHandler *getHandler();


		static void validate_pass(Client &sender, const std::vector<std::string> &arguments);
		static void validate_user(Client &sender, const std::vector<std::string> &arguments);
		static void validate_nick(Client &sender, const std::vector<std::string> &arguments);
		static void validate_ping(Client &sender, const std::vector<std::string> &arguments);
		static void validate_pong(Client &sender, const std::vector<std::string> &arguments);
		static void validate_privmsg(Client &sender, const std::vector<std::string> &arguments);
		static void validate_join(Client &sender, const std::vector<std::string> &arguments);
		static void validate_part(Client &sender, const std::vector<std::string> &arguments);
		static void validate_topic(Client &sender, const std::vector<std::string> &arguments);
		static void validate_kick(Client &sender, const std::vector<std::string> &arguments);
		static void validate_mode(Client &sender, const std::vector<std::string> &arguments);
		static void validate_who(Client &sender, const std::vector<std::string> &arguments);
		static void validate_bot(Client &sender, const std::vector<std::string> &arguments);
		static void execute_pass(Client &sender, const std::vector<std::string> &arguments);
		static void execute_user(Client &sender, const std::vector<std::string> &arguments);
		static void execute_nick(Client &sender, const std::vector<std::string> &arguments);
		static void execute_ping(Client &sender, const std::vector<std::string> &arguments);
		static void execute_pong(Client &sender, const std::vector<std::string> &arguments);
		static void execute_privmsg(Client &sender, const std::vector<std::string> &arguments);
		static void execute_notice(Client &sender, const std::vector<std::string> &arguments);
		static void execute_join(Client &sender, const std::vector<std::string> &arguments);
		static void execute_invite(Client &sender, const std::vector<std::string> &arguments);
		static void execute_topic(Client &sender, const std::vector<std::string> &arguments);
		static void execute_part(Client &sender, const std::vector<std::string> &arguments);
		static void execute_kick(Client &sender, const std::vector<std::string> &arguments);
		static void execute_quit(Client &sender, const std::vector<std::string> &arguments);
		static void execute_mode(Client &sender, const std::vector<std::string> &arguments);
		static void execute_who(Client &sender, const std::vector<std::string> &arguments);
		static void execute_bot(Client &sender, const std::vector<std::string> &arguments);
		static void execute_botme(Client &sender, const std::vector<std::string> &arguments);
		static void execute_ftp(Client &sender, const std::vector<std::string> &arguments);
};

#endif