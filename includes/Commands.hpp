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

class Command
{
	private:
		static	Command *instance;
	public:
		Command();
		~Command();
		static	Command *getCommand();

		typedef void (*command_pointer)(Client &, const std::vector<std::string> &);
		void		execute(Client &sender, const std::vector<std::string> &arguments, command_pointer comm);
		static void validate_pass(Client &sender, const std::vector<std::string> &arguments);
		static void validate_user(Client &sender, const std::vector<std::string> &arguments);
		static void validate_nick(Client &sender, const std::vector<std::string> &arguments);
		static void validate_ping(Client &sender, const std::vector<std::string> &arguments);
		static void validate_pong(Client &sender, const std::vector<std::string> &arguments);
		static void validate_privmsg(Client &sender, const std::vector<std::string> &arguments);
		static void validate_join(Client &sender, const std::vector<std::string> &arguments);
		static void validate_part(Client &sender, const std::vector<std::string> &arguments);
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
		static void execute_part(Client &sender, const std::vector<std::string> &arguments);
		static void execute_kick(Client &sender, const std::vector<std::string> &arguments);
		static void execute_quit(Client &sender, const std::vector<std::string> &arguments);
		static void execute_mode(Client &sender, const std::vector<std::string> &arguments);
		static void execute_who(Client &sender, const std::vector<std::string> &arguments);
		static void execute_bot(Client &sender, const std::vector<std::string> &arguments);
		static void execute_botme(Client &sender, const std::vector<std::string> &arguments);
		static void execute_ftp(Client &sender, const std::vector<std::string> &arguments);

};


#endif // COMMANDS_HPP