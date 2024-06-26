#include "CommandHandler.hpp"


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
	delete instance;
}

void	ValidateChannelMode(const Client &sender, const std::vector<std::string> &arguments)
{
	Server *server = Server::getServer();
	MessageController *messageController = MessageController::getController();
	ClientManager	*clientManager = ClientManager::getManager();

	std::string channelName = messageController->GetChannelName(arguments[0]);
	if(server->HasChannel(channelName) == false)
		throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403);
	Channel &channel = server->getChannel(channelName);
	if (arguments.size() == 1)
		return ;

	std::string modeString = arguments[1];
	size_t index = modeString.find_first_not_of("+-wrioktlb");
	if (index != std::string::npos)
		throw IRCException(sender.getNick(), " " + modeString + " :is unknown mode char to me", 472);
	int plusSign  = messageController->SignCount(modeString, '+');
	int minusSign = messageController->SignCount(modeString, '-');
	if ((plusSign != 0 && plusSign != 1) ||
			(minusSign != 0 && minusSign != 1))
		throw IRCException(sender.getNick(), " " + modeString + " :is unknown mode char to me", 472);
	if (*modeString.rbegin() == '+' || *modeString.rbegin() == '-')
		throw IRCException(sender.getNick(), " " + modeString + " :is unknown mode char to me", 472);

	std::string addingModes = messageController->GetModesString(modeString, '+');
	std::string removingModes = messageController->GetModesString(modeString, '-');

	std::cout << "ADDING modes: " << addingModes << std::endl;
	std::cout << "REMOVING modes: " << removingModes << std::endl;
	for(size_t i = 0; i < addingModes.length(); ++i)
	{
		char set = addingModes[i];
		if(set == 'o' || set == 'k')
		{
			if(channel.IsAdmin(sender.getSocket()) == false)
				throw IRCException(sender.getNick(), " :Cant change mode for other users", 502);
			if (arguments.size() < 3)
				throw IRCException(sender.getNick(), " MODE :Not enough parameters", 461);
		}
		int clientSocket = clientManager->GetClientSocket(arguments[i + 2]);
		if (set == 'o' && !channel.HasMember(clientSocket))
			throw IRCException(sender.getName(), " " + sender.getNick() + " " + channelName + " They aren't on that channel", 441);
	}
	for(size_t i = 0; i < removingModes.length(); ++i)
	{
		char set = removingModes[i];
		if(set == 'o')
		{
			if(channel.IsAdmin(sender.getSocket()) == false)
				throw IRCException(sender.getNick(), " :Cant change mode for other users", 502);
			if (arguments.size() < 3)
				throw IRCException(sender.getNick(), " MODE :Not enough parameters", 461);
			int clientSocket = clientManager->GetClientSocket(arguments[i + 2]);
			if (!channel.HasMember(clientSocket))
				throw IRCException(sender.getName(), " " + sender.getNick() + " " + channelName + " They aren't on that channel", 441);
		}
	}
}

void CommandHandler::validate_pass(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed())
		throw IRCException(sender.getNick(), " :You may not reregister", 462);
	if(arguments.size() == 0)
		throw  IRCException(sender.getNick(), " PASS :Not enough parameters", 461);
	if (Server::getServer()->getPass() != arguments[0])
		throw IRCException(sender.getNick(), " :Password incorrect", 464);
}

void CommandHandler::validate_user(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	if(arguments.size() < 4)
		throw IRCException(sender.getNick(), " USER :Not enough parameters", 461);
	if(sender.getIsUsered())
		throw IRCException(sender.getNick(), " :You may not reregister", 462);
}

void CommandHandler::validate_nick(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	if (arguments.size() == 0)
		throw IRCException(sender.getName(), " :No nickname given", 431);
	if (ClientManager::getManager()->HasClient(arguments[0]))
		throw IRCException(arguments[0], " " + sender.getNick() + " :Nickname is already in use", 433);
	if (!MessageController::getController()->IsValidNickname(arguments[0]))
		throw  IRCException(sender.getNick(), " " +  arguments[0] + " :Erroneus nickname", 432);
}

void CommandHandler::validate_ping(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PING :Not enough parameters", 461);
}

void CommandHandler::validate_pong(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PONG :Not enough parameters", 461);
}

void CommandHandler::validate_privmsg(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	MessageController *messageController = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	if(arguments.size() <= 1)
		throw IRCException(sender.getNick(), " PRIVMSG :Not enough parameters", 461);
	for (size_t i = 0; i < args.size(); i++)
	{
		std::string channelName = messageController->GetChannelName(args[i]);

		if(server->HasChannel(channelName))
		{
			if(!(server->getChannel(channelName).HasMode(ModeType::write_)))
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot send to channel", 404);
			if(!server->IsBot(sender) && !(server->getChannel(channelName).HasMember(sender.getSocket())))
				throw IRCException(sender.getNick(), " PRIVMSG :No such nick/channel", 401);
		}
		else if(client_managar->HasClient(args[i]) == false)
			throw IRCException(sender.getNick(), " " + args[i] + " :No such nick/channel", 401);

	}
}

void CommandHandler::validate_join(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " JOIN :Not enough parameters", 461);
	Server *server =  Server::getServer();
	MessageController *messageController = MessageController::getController();
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if (!messageController->IsValidChannelName(args[i]))
			throw IRCException(sender.getNick(), " " + args[i] + " :Bad channel mask", 476);
		std::string channelName = messageController->GetChannelName(args[i]);
		if (server->HasChannel(channelName))
		{
			Channel	channel = server->getChannel(channelName);
			if (channel.HasMode(ModeType::user_limit) && channel.getMemberCount() == channel.size())
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+l)", 471);
			if (channel.HasMode(ModeType::invite))
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+i)", 473);
			if (channel.HasMode(ModeType::private_) && !channel.CheckPassword(arguments[1]))
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+k)", 475);
		}
	}
}

void CommandHandler::validate_topic(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;
	if (arguments.empty())
		throw IRCException(sender.getNick(), " TOPIC :Not enough parameters", 461);
	MessageController *messageController = MessageController::getController();
	Server *server = Server::getServer();
	std::vector<std::string> chans = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < chans.size(); i++)
	{
		std::string channelName = messageController->GetChannelName(chans[i]);
		if(server->HasChannel(channelName) == false) //stugel karox e chans[i] @ndunel
			throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403);
		Channel channel = server->getChannel(channelName);
		if(channel.HasMember(sender.getSocket()) == false)
			throw  IRCException(sender.getNick(), " " + channelName + " :You're not on that channel", 442);
		if (channel.HasMode(ModeType::topic) == false)
			throw IRCException(sender.getNick(), " " + channelName + " :TOPIC mode is OFF ('MODE <channel> +t' is needed)", 477);
	}
}

void CommandHandler::validate_part(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;
	Server *server = Server::getServer();
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PART :Not enough parameters", 461);
	MessageController *messageController = MessageController::getController();
	std::vector<std::string> channels = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!messageController->IsValidChannelName(channels[i]))
			throw IRCException(sender.getNick(), " " + channels[i] + " :Bad channel mask", 476);
		std::string channelName = messageController->GetChannelName(channels[i]);
		if(!server->HasChannel(channelName))
			throw IRCException(sender.getNick(), " " + channels[i] + " :No such channel", 403);
		Channel channel = server->getChannel(channelName);
		if(channel.HasMember(sender.getSocket()) == false)
			throw  IRCException(sender.getNick(), " " + channelName + " :You're not on that channel", 442);
	}
}

void CommandHandler::validate_kick(Client &sender, const std::vector<std::string> &arguments)
{
	std::string chName = arguments[0];
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;
	if(arguments.size() != 2)
		throw IRCException(sender.getNick(), " KICK :Not enough parameters", 461);
	if (!MessageController::getController()->IsValidChannelName(chName))
		throw IRCException(sender.getNick(), " " + chName + " :Bad channel mask", 476);
	std::string channelName = MessageController::getController()->GetChannelName(chName);
	Server *server = Server::getServer();
	if(server->HasChannel(channelName) == false)
		throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403);
}

void CommandHandler::validate_mode(Client &sender, const std::vector<std::string> &arguments)
{
	if (arguments.empty())
		throw IRCException(sender.getNick(), " MODE :Not enough parameters", 461);
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;

	ClientManager *clientManager = ClientManager::getManager();
	MessageController *messageController = MessageController::getController();

	if (messageController->IsValidChannelName(arguments[0]))
	{
		ValidateChannelMode(sender, arguments);
	}
	else if (clientManager->HasClient(arguments[0]))
	{
		if (sender.getNick() != arguments[0])
			throw IRCException(arguments[0], " :Cant change mode for other users", 502);
	}
	else
		throw IRCException(sender.getNick(), " " + arguments[0] + " :No such nick/channel", 401);
}

void CommandHandler::validate_who(Client &sender, const std::vector<std::string> &arguments)
{
	(void) arguments;
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " WHO :Not enough parameters", 461);
	std::string target = arguments[0];
	MessageController *controller = MessageController::getController();
	Server *server = Server::getServer();
	if(controller->IsValidChannelName(target))
	{
		std::string channelName = controller->GetChannelName(arguments[0]);
		if(server->HasChannel(channelName) == false)
			throw IRCException(sender.getNick(), " " + arguments[0] + " :No such channel", 403);
		if(server->getChannel(channelName).HasMember(sender.getSocket()) == false)
			throw IRCException(sender.getNick(), " " + arguments[0] + " :You're not on that channel", 442);
	}
	if(ClientManager::getManager()->HasClient(sender.getSocket()) == false)
		throw IRCException(sender.getNick(), " WHO :No such nick/channel", 401);
}

void CommandHandler::validate_bot(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ;
	Server *server = Server::getServer();
	MessageController *controller = MessageController::getController();
	ClientManager *manager = ClientManager::getManager();
	if (arguments.size() == 0)
		throw IRCException(sender.getNick(), " /bot :Not enough parameters", 461);
	if (!server->IsBotConnected())
		throw IRCException(sender.getNick(), " :No bot has been connected", 7777);
	if (arguments.size() == 1)
		return ;
	std::string channelName = controller->GetChannelName(arguments[1]);
	if (controller->IsValidChannelName(arguments[1]))
	{
		if (!server->HasChannel(channelName))
			throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403);
		Channel &channel = server->getChannel(channelName);
		if (!channel.HasMember(sender.getSocket()))
			throw IRCException(sender.getName(), " " + sender.getNick() + " " + channelName + " They aren't on that channel", 441);
	}
	else if (!manager->HasClient(arguments[1]))
		throw IRCException(sender.getNick(), " " + channelName + " :No such nick/channel", 401);
}

void CommandHandler::execute_pass(Client &sender, const std::vector<std::string> &arguments)
{
	validate_pass(sender, arguments);
	(void) arguments;
	sender.setIsPassed(true);
}

void CommandHandler::execute_user(Client &sender, const std::vector<std::string> &arguments)
{
	validate_user(sender, arguments);
	Server *server = Server::getServer();
	sender.setName(arguments[0]);
	sender.setIsUsered(true);
	if(sender.isDone())
		server->SendHelloMessage(sender);
}

void CommandHandler::execute_nick(Client &sender, const std::vector<std::string> &arguments)
{
	validate_nick(sender, arguments);
	Server *server = Server::getServer();
	sender.setNick(arguments[0]);
	sender.setIsNicked(true);
	if(sender.isDone())
		server->SendHelloMessage(sender);
}

void CommandHandler::execute_ping(Client &sender, const std::vector<std::string> &arguments)
{
	validate_ping(sender, arguments);
	sender.SendPongMessage(arguments[0]);
}

void CommandHandler::execute_pong(Client &sender, const std::vector<std::string> &arguments)
{
	validate_pong(sender, arguments);
	sender.SendPongMessage(arguments[0]);
}

void CommandHandler::execute_privmsg(Client &sender, const std::vector<std::string> &arguments)
{
 	validate_privmsg(sender, arguments);
	MessageController *messageController = MessageController::getController();
	ClientManager *clientManager = ClientManager::getManager();
	Server *server = Server::getServer();
	std::string MessageBody = "";
	for (size_t i = 1; i < arguments.size(); i++)
		MessageBody = MessageBody + arguments[i] + " ";

	std::vector<std::string> args = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{

		std::string channelName = messageController->GetChannelName(args[i]);
		if (server->HasChannel(channelName))
			server->getChannel(channelName).Broadcast(sender, MessageBody, "PRIVMSG");
		else
		{
			sender.SendMessage(clientManager->getClient(args[i]),
					"PRIVMSG", MessageBody);
			if (arguments[1].find("DCC") != std::string::npos)
			{

				std::vector<std::string> params = messageController->Split(arguments[1], " ");
				if(params.size() < 3)
					return ;
				int port = custom_stoi(params[4]);
				int sockfd = socket(AF_INET, SOCK_STREAM, 0);
				fcntl(sockfd, F_SETFL, O_NONBLOCK);
				struct sockaddr_in address;
				uint32_t ip_int = params.size() > 2 ? custom_stoi(params[3]) : 2130706433;
				struct in_addr addr;
				addr.s_addr = htonl(ip_int);
				std::string ip_str = inet_ntoa(addr);
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = inet_addr("127.0.0.1");
				address.sin_port = htons(port);
				if (connect(sockfd,(struct sockaddr *)&address, sizeof(address)) != 0)
				{
					perror("Connect");
				}
				std::string buffer;
				std::string response_2 =  "DCC GET " + ip_str + " " + to_string(port) + " " + params[2] + " " + to_string(0) + "\r\n";
				if (send(sender.getSocket(), response_2.c_str(), response_2.length() + 1, 0) != 0)
				{
					perror("Send");
				}
			}
		}
	}
}

void CommandHandler::execute_notice(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		return ;
	MessageController *messageController = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();

	if(arguments.size() <= 1)
		return ;
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	std::string MessageBody = "";

	for (size_t i = 1; i < arguments.size(); i++)
		MessageBody = MessageBody + arguments[i] + " ";

	for (size_t i = 0; i < args.size(); i++)
	{
		bool isValidChannel = messageController->IsValidChannelName(args[i]);
		std::string channelName = messageController->GetChannelName(args[i]);
		if(isValidChannel && server->HasChannel(channelName))
		{
			Channel channel = server->getChannel(channelName);
			if (channel.HasMember(sender.getSocket()))
				channel.Broadcast(sender, MessageBody, "NOTICE");
		}
		else if(client_managar->HasClient(args[i]))
		{
			sender.SendMessage(client_managar->getClient(args[i]),
					"NOTICE", arguments.back());
		}
		else return;
	}
}

void CommandHandler::execute_join(Client &sender, const std::vector<std::string> &arguments)
{
	validate_join(sender, arguments);
	Server *server = Server::getServer();
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		std::string channelName = message->GetChannelName(args[i]);
		if (server->HasChannel(args[i]))
		{
			Channel &channel = server->getChannel(channelName);
			channel.AddMember(sender.getSocket());
		}
		else
		{
			Channel &channel = server->getChannel(channelName);
			channel.AddMode(ModeType::write_ | ModeType::topic);
			channel.AddMember(sender.getSocket());
			if (arguments.size() > 1)
			{
				channel.SetPassword(arguments[1]);
				channel.AddMode(ModeType::private_);
			}
		}
		Channel &channel = server->getChannel(channelName);
		channel.ChannelJoinResponse(sender);
	}
}

void CommandHandler::execute_invite(Client &sender, const std::vector<std::string> &arguments)
{
	std::string channelName;
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	if(arguments.size() < 2)
		throw IRCException(sender.getNick(), " INVITE :Not enough parameters", 461);
	MessageController *messageController = MessageController::getController();
	ClientManager *clientManager = ClientManager::getManager();
	Server *server = Server::getServer();
	std::vector<std::string> chans = messageController->Split(arguments[1],",");
	for (size_t i = 0; i < chans.size(); i++)
	{
		channelName = messageController->GetChannelName(chans[i]);
		if(!(server->getChannel(channelName).IsAdmin(sender.getSocket())))
			throw IRCException(sender.getNick(), " " + channelName + " :You're not channel operator", 482);
	}
	std::vector<std::string> users = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < users.size(); i++)
	{
		if(!clientManager->HasClient(users[i]))
			throw IRCException(sender.getNick(), " " + users[i] + " :No such nick/channel", 401);
		for (size_t j = 0; j < chans.size(); j++)
		{
			channelName = messageController->GetChannelName(chans[j]);
			if (server->HasChannel(channelName))
			{
				Channel &channel = server->getChannel(channelName);
				if (channel.HasMode(ModeType::user_limit) && channel.getMemberCount() == channel.size())
					throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+l)", 471);
				channel.AddMember(clientManager->GetClientSocket(users[i]));
				channel.ChannelJoinResponse(clientManager->getClient(users[i]));
			}
		}
	}
}

void CommandHandler::execute_topic(Client &sender, const std::vector<std::string> &arguments)
{
	validate_topic(sender, arguments);
	std::string message;
	std::string channelName;
	MessageController *messageController = MessageController::getController();
	Server *server = Server::getServer();
	std::vector<std::string> chans = messageController->Split(arguments[0],",");
	if (arguments.size() < 2)
	{
		for (size_t i = 0; i < chans.size(); i++)
		{
			std::string channelName = messageController->GetChannelName(chans[i]);
			Channel &channel = server->getChannel(channelName);
			std::string topic = channel.getTopic();
			message = sender.GetFormattedText() + " #" + channelName + " :" + ((topic.empty() || !channel.HasMode(ModeType::topic))? "No topic is set":topic);
			// you are not on the channel
			SendMessageToClient(sender, message);
		}
	}
	else
	{
		for (size_t i = 0; i < chans.size(); i++)
		{
			channelName = messageController->GetChannelName(chans[i]);
			Channel &channel = server->getChannel(channelName);
			if(!(channel.IsAdmin(sender.getSocket())))
				throw IRCException(sender.getNick(), " " + channelName + " :You're not channel operator", 482);
		}
		for (size_t i = 0; i < chans.size(); i++)
		{
			channelName = messageController->GetChannelName(chans[i]);
			Channel &channel= server->getChannel(channelName);
			if (channel.HasMode(ModeType::topic))
				channel.setTopic(arguments[1]);
			else
				SendMessageToClient(sender, sender.GetFormattedText() + " #" + channelName + " :No topic is set");
		}
	}
}

void CommandHandler::execute_part(Client &sender, const std::vector<std::string> &arguments)
{
	validate_part(sender, arguments);
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	std::string channelName;
	for (size_t i = 0; i < args.size(); i++)
	{
		Server *server = Server::getServer();
		channelName = message->GetChannelName(args[i]);

		Channel &channel = server->getChannel(channelName);
		PartMessage(sender,channelName);
		channel.LeaveMember(sender.getSocket());
		if(channel.getMemberCount() == 0)
			server->removeChannel(channelName);
	}
}

void CommandHandler::execute_kick(Client &sender, const std::vector<std::string> &arguments)
{
	validate_kick(sender, arguments);
	Server *server = Server::getServer();
	ClientManager *clientManager = ClientManager::getManager();
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Channel &channel = server->getChannel(channelName);
	int	memberSocket = clientManager->GetClientSocket(arguments[1]);
	channel.KickMember(sender.getSocket(), memberSocket);
	KickMessage(clientManager->getClient(arguments[1]),channelName,sender.getNick());
}

void CommandHandler::execute_quit(Client &sender, const std::vector<std::string> &arguments)
{
	ClientManager *manager = ClientManager::getManager();
	int socket = sender.getSocket();
	std::string reason = arguments.size() == 0 ? " no reason " : arguments[0];
	manager->CloseClient(socket, reason);
	manager->RemoveClient(socket);
}

void CommandHandler::execute_mode(Client &sender, const std::vector<std::string> &arguments)
{
	validate_mode(sender, arguments);
	MessageController	*messageController = MessageController::getController();
	Server	*server =  Server::getServer();
	std::string target = arguments[0];

	if (messageController->IsValidChannelName(target))
	{
		std::string channelName = messageController->GetChannelName(arguments[0]);
		Channel &channel = server->getChannel(channelName);
		if (arguments.size() == 1)
		{
			ChannelModeMessage(sender, channelName);
			return ;
		}
		std::string modeString = arguments[1];
		std::string addingModes = messageController->GetModesString(modeString, '+');
		std::string removingModes = messageController->GetModesString(modeString, '-');
		for (int i = 0; addingModes[i]; ++i)
		{
			char mode = addingModes[i];

			if (mode == 'w')
				channel.AddMode(ModeType::write_);
			else if (mode == 'r')
				channel.AddMode(ModeType::read);
			else if (mode == 'i')
				channel.AddMode(ModeType::invite);
			else if (mode == 't')
				channel.AddMode(ModeType::topic);
			else if (mode == 'l')
			{
				int new_limit = custom_stoi(arguments[2]);
				if (channel.getMemberCount() > new_limit)
					throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+l)", 471);
				channel.SetLimit(new_limit);
			}
			else if (mode == 'k')
			{
				channel.AddMode(ModeType::private_);
				channel.SetPassword(arguments[2]);
			}
			else if(mode == 'o')
			{
				int	modifyingClientSocket = ClientManager::getManager()->GetClientSocket(arguments[2]);
				channel.MakeAdmin(sender.getSocket(), modifyingClientSocket);
			}
		}
		for (int i = 0; removingModes[i]; ++i)
		{
			char mode = removingModes[i];
			if (mode == 'w')
				channel.RemoveMode(ModeType::write_);
			else if (mode == 'r')
				channel.RemoveMode(ModeType::read);
			else if (mode == 'i')
				channel.RemoveMode(ModeType::invite);
			else if (mode == 't')
				channel.RemoveMode(ModeType::topic);
			else if (mode == 'l')
				channel.SetLimit(0);
			else if (mode == 'k')
			{
				channel.RemoveMode(ModeType::private_);
				channel.SetPassword("");
			}
			else if(mode == 'o')
			{
				int	modifyingClientSocket = ClientManager::getManager()->GetClientSocket(arguments[2]);
				channel.RemoveFromAdmins(sender.getSocket(), modifyingClientSocket);
			}
		}
	}
	else{
		UserModeMessage(sender);
	}
}

void CommandHandler::execute_who(Client &sender, const std::vector<std::string> &arguments)
{
	validate_who(sender, arguments);
	std::string target = arguments[0];
	MessageController *controller = MessageController::getController();
	Server *server = Server::getServer();
	if(controller->IsValidChannelName(target))
	{
		std::string channelName = controller->GetChannelName(arguments[0]);
		server->getChannel(channelName).ChannelWhoResponse(sender);
		return ;
	}
}

void CommandHandler::execute_bot(Client &sender, const std::vector<std::string> &arguments)
{
	validate_bot(sender, arguments);
	std::string receiver;
	if (arguments.size() == 1)
		receiver = sender.getNick();
	else 
		receiver = arguments[1];
	Server::getServer()->SendMessageToBot(arguments[0] + " " + receiver);
}

void CommandHandler::execute_botme(Client &sender, const std::vector<std::string> &arguments)
{
	(void) arguments;
	Server::getServer()->SetBotDescriptor(sender.getSocket());
}

void CommandHandler::execute_ftp(Client &sender, const std::vector<std::string> &arguments)
{
	std::string defaultFileName = "Makefile";
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451);
	std::ifstream input(arguments.size() >= 1 ? arguments[0].c_str() : defaultFileName.c_str());

	if (!input) 
	{
		SendMessageToClient(sender, "Cannot Open the given File");
		return ;
	}

	std::string line;
	std::string mem;
	while (std::getline(input, line)) 
	{
		mem +=line;
	}
	SendMessageToClient(sender, mem);
}

void	CommandHandler::InitilizeCommands()
{
	commands["PASS"] = &execute_pass;
	commands["NICK"] = &execute_nick;
	commands["USER"] = &execute_user;
	commands["PING"] = &execute_ping;
	commands["PONG"] = &execute_pong;
	commands["PRIVMSG"] = &execute_privmsg;
	commands["NOTICE"] = &execute_notice;
	commands["JOIN"] = &execute_join;
	commands["INVITE"] = &execute_invite;
	commands["TOPIC"] = &execute_topic;
	commands["PART"] = &execute_part;
	commands["KICK"] = &execute_kick;
	commands["QUIT"] = &execute_quit;
	commands["MODE"] = &execute_mode;
	commands["WHO"] = &execute_who;
	commands["BOT"] = &execute_bot;
	commands["BOTME"] = &execute_botme;
	commands["FTP"] = &execute_ftp;
}

void	CommandHandler::ExecuteCommand(Client &sender, const CommandData &data)
{
	if (commands.find(data.command) != commands.end())
	{
		commands[data.command](sender, data.args);
	}
	else
	{
		throw IRCException(sender.getNick(), " " + data.command + " :Unknown command", 421);
	}
}
