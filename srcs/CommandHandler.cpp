#include "CommandHandler.hpp"
#include "Exceptions.hpp"

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
		throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403); //NoSuchChannel(sender.getNick(),channelName);
	Channel &channel = server->getChannel(channelName);
	if (arguments.size() == 1)
		return ;

	std::string modeString = arguments[1];
	size_t index = modeString.find_first_not_of("+-wriokb");
	if (index != std::string::npos)
		throw IRCException(sender.getNick(), " " + modeString + " :is unknown mode char to me", 472); //UnknownMode(sender.getNick(), modeString);
	int plusSign  = messageController->SignCount(modeString, '+');
	int minusSign = messageController->SignCount(modeString, '-');
	if ((plusSign != 0 && plusSign != 1) ||
			(minusSign != 0 && minusSign != 1))
		throw IRCException(sender.getNick(), " " + modeString + " :is unknown mode char to me", 472); //UnknownMode(sender.getNick(), modeString);
	if (*modeString.rbegin() == '+' || *modeString.rbegin() == '-')
		throw IRCException(sender.getNick(), " " + modeString + " :is unknown mode char to me", 472); //UnknownMode(sender.getNick(), modeString);

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
				throw IRCException(sender.getNick(), " :Cant change mode for other users", 502); //UsersDontMatch(sender.getNick());
			if (arguments.size() < 3)
				throw IRCException(sender.getNick(), " MODE :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"MODE");
		}
		int clientSocket = clientManager->GetClientSocket(arguments[i + 1]);
		if (set == 'o' && !channel.HasMember(clientSocket))
			throw IRCException(sender.getName(), " " + sender.getNick() + " " + channelName + " They aren't on that channel", 441); //UserNotInChannel(sender.getName(),sender.getNick(), channelName);
	}
	for(size_t i = 0; i < removingModes.length(); ++i)
	{
		char set = removingModes[i];
		if(set == 'o')
		{
			if(channel.IsAdmin(sender.getSocket()) == false)
				throw IRCException(sender.getNick(), " :Cant change mode for other users", 502); //UsersDontMatch(sender.getNick());
			if (arguments.size() < 3)
				throw IRCException(sender.getNick(), " MODE :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"MODE");
			int clientSocket = clientManager->GetClientSocket(arguments[i + 1]);
			if (!channel.HasMember(clientSocket))
				throw IRCException(sender.getName(), " " + sender.getNick() + " " + channelName + " They aren't on that channel", 441); //UserNotInChannel(sender.getName(),sender.getNick(), channelName);
		}
	}
}

void CommandHandler::validate_pass(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed())
		throw IRCException(sender.getNick(), " :You may not reregister", 462);//AlreadyRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw  IRCException(sender.getNick(), " PASS :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"PASS");
	if (Server::getServer()->getPass() != arguments[0])
		throw IRCException(sender.getNick(), " :Password incorrect", 464);//PasswordMissmatch(sender.getNick());
}
void CommandHandler::validate_user(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() < 4)
		throw IRCException(sender.getNick(), " USER :Not enough parameters", 461); // NeedMoreParams(sender.getNick(),"USER");
	if(sender.getIsUsered())
		throw IRCException(sender.getNick(), " :You may not reregister", 462); //AlreadyRegistered(sender.getNick());
}
void CommandHandler::validate_nick(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if (arguments.size() == 0)
		throw IRCException(sender.getName(), " :No nickname given", 431);//NoNickNameGiven(sender.getName());
	if (ClientManager::getManager()->HasClient(arguments[0]))
		throw IRCException(arguments[0], " " + sender.getNick() + " :Nickname is already in use", 433); //NicknameInUse(arguments[0],sender.getNick());
	if (!MessageController::getController()->IsValidNickname(arguments[0]))
		throw  IRCException(sender.getNick(), " " +  arguments[0] + " :Erroneus nickname", 432); //ErroneusNickname(sender.getNick(), arguments[0]);
}
void CommandHandler::validate_ping(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PING :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"PING");
}
void CommandHandler::validate_pong(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PONG :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"PONG");
}
void CommandHandler::validate_privmsg(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	MessageController *messageController = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	if(arguments.size() <= 1)
		throw IRCException(sender.getNick(), " PRIVMSG :Not enough parameters", 461); // NeedMoreParams(sender.getNick(),"PRIVMSG");
	for (size_t i = 0; i < args.size(); i++)
	{
		std::string channelName = messageController->GetChannelName(args[i]);

		if(server->HasChannel(channelName))
		{
			if(!(server->getChannel(channelName).HasMode(ModeType::write_)))
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot send to channel", 404); //CannotSendToChannel(sender.getNick(),channelName);
			if(!server->IsBot(sender) && !(server->getChannel(channelName).HasMember(sender.getSocket())))
				throw IRCException(sender.getNick(), " PRIVMSG :No such nick/channel", 401); //NoSuchNick(sender.getNick(),"PRIVMSG");
		}
		else if(client_managar->HasClient(args[i]) == false)
			throw IRCException(sender.getNick(), " " + args[i] + " :No such nick/channel", 401); //NoSuchNick(sender.getNick(),args[i]);

	}
}
void CommandHandler::validate_join(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " JOIN :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"JOIN");
	Server *server =  Server::getServer();
	MessageController *messageController = MessageController::getController();
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if (!messageController->IsValidChannelName(args[i]))
			throw IRCException(sender.getNick(), " " + args[i] + " :Bad channel mask", 476); //BadChannelMask(sender.getNick(), args[i]);
		std::string channelName = messageController->GetChannelName(args[i]);
		if (server->HasChannel(channelName))
		{
			Channel	channel = server->getChannel(channelName);
			if (channel.HasMode(ModeType::invite))
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+i)", 473); //InviteOnlyChannel(sender.getNick(),channelName);
			if (channel.HasMode(ModeType::private_) && !channel.CheckPassword(arguments[1]))
				throw IRCException(sender.getNick(), " " + channelName + " :Cannot join channel (+k)", 475); //BadChannelKey(sender.getNick(), channelName);
		}
	}
}
void CommandHandler::validate_part(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	Server *server = Server::getServer();
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PART :Not enough parameters", 461);// NeedMoreParams(sender.getNick(),"PART");
	MessageController *messageController = MessageController::getController();
	std::vector<std::string> channels = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!messageController->IsValidChannelName(channels[i]))
			throw IRCException(sender.getNick(), " " + channels[i] + " :Bad channel mask", 476); //BadChannelMask(sender.getNick(), channels[i]);
		std::string channelName = messageController->GetChannelName(channels[i]);
		if(!server->HasChannel(channelName))
			throw IRCException(sender.getNick(), " " + channels[i] + " :No such channel", 403); //NoSuchChannel(sender.getNick(),channels[i]);
		Channel channel = server->getChannel(channelName);
		if(channel.HasMember(sender.getSocket()) == false)
			throw  IRCException(sender.getNick(), " " + channelName + " :You're not on that channel", 442); //NotOnChannel(sender.getNick(),channelName);
	}
}
void CommandHandler::validate_kick(Client &sender, const std::vector<std::string> &arguments)
{
	std::string chName = arguments[0];
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() != 2)
		throw IRCException(sender.getNick(), " KICK :Not enough parameters", 461); // NeedMoreParams(sender.getNick(),"KICK");
	if (!MessageController::getController()->IsValidChannelName(chName))
		throw IRCException(sender.getNick(), " " + chName + " :Bad channel mask", 476); //BadChannelMask(sender.getNick(), chName);
	std::string channelName = MessageController::getController()->GetChannelName(chName);
	Server *server = Server::getServer();
	if(server->HasChannel(channelName) == false)
		throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403); //NoSuchChannel(sender.getNick(),channelName);
}
void CommandHandler::validate_mode(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());

	ClientManager *clientManager = ClientManager::getManager();
	MessageController *messageController = MessageController::getController();

	if (messageController->IsValidChannelName(arguments[0]))
	{
		ValidateChannelMode(sender, arguments);
	}
	else if (clientManager->HasClient(arguments[0]))
	{
		if (sender.getNick() != arguments[0])
			throw IRCException(arguments[0], " :Cant change mode for other users", 502); //UsersDontMatch(arguments[0]);
	}
	else
		throw IRCException(sender.getNick(), " " + arguments[0] + " :No such nick/channel", 401); //NoSuchNick(sender.getNick(), arguments[0]);
}
void CommandHandler::validate_who(Client &sender, const std::vector<std::string> &arguments)
{
	(void) arguments;
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " WHO :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"WHO");
	std::string target = arguments[0];
	MessageController *controller = MessageController::getController();
	Server *server = Server::getServer();
	if(controller->IsValidChannelName(target))
	{
		std::string channelName = controller->GetChannelName(arguments[0]);
		if(server->HasChannel(channelName) == false)
			throw IRCException(sender.getNick(), " " + arguments[0] + " :No such channel", 403); //NoSuchChannel(sender.getNick(),arguments[0]);
		if(server->getChannel(channelName).HasMember(sender.getSocket()) == false)
			throw IRCException(sender.getNick(), " " + arguments[0] + " :You're not on that channel", 442); //NotOnChannel(sender.getNick(),arguments[0]);
	}
	if(ClientManager::getManager()->HasClient(sender.getSocket()) == false)
		throw IRCException(sender.getNick(), " WHO :No such nick/channel", 401);//NoSuchNick(sender.getNick(),"WHO");
}
void CommandHandler::validate_bot(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	Server *server = Server::getServer();
	MessageController *controller = MessageController::getController();
	ClientManager *manager = ClientManager::getManager();
	if (arguments.size() == 0)
		throw IRCException(sender.getNick(), " /bot :Not enough parameters", 461); //NeedMoreParams(sender.getNick(), "/bot");
	if (!server->IsBotConnected())
		throw IRCException(sender.getNick(), " :No bot has been connected", 7777); // NoBotConnected(sender.getNick());
	if (arguments.size() == 1)
		return ;
	std::string channelName = controller->GetChannelName(arguments[1]);
	if (controller->IsValidChannelName(arguments[1]))
	{
		if (!server->HasChannel(channelName))
			throw IRCException(sender.getNick(), " " + channelName + " :No such channel", 403); //NoSuchChannel(sender.getNick(), channelName);
		Channel &channel = server->getChannel(channelName);
		if (!channel.HasMember(sender.getSocket()))
			throw IRCException(sender.getName(), " " + sender.getNick() + " " + channelName + " They aren't on that channel", 441); //UserNotInChannel(sender.getName(),sender.getNick(), channelName);
	}
	else if (!manager->HasClient(arguments[1]))
		throw IRCException(sender.getNick(), " " + channelName + " :No such nick/channel", 401);// NoSuchNick(sender.getNick(), channelName);
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
				int port = std::stoi(params[4]);
				int sockfd = socket(AF_INET, SOCK_STREAM, 0);
				int flags = fcntl(sockfd, F_GETFL, 0);
				fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
				struct sockaddr_in address;
				uint32_t ip_int = params.size() > 2 ? std::stoi(params[3]) : 2130706433;
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
				std::string response_2 =  "DCC GET " + ip_str + " " + std::to_string(port) + " " + params[2] + " " + std::to_string(0) + "\r\n";
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
			channel.AddMember(sender.getSocket());
			channel.AddMode(ModeType::write_);
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
void CommandHandler::execute_part(Client &sender, const std::vector<std::string> &arguments)
{
	validate_part(sender, arguments);
	// validate(sender,arguments);
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	std::string channelName;
	for (size_t i = 0; i < args.size(); i++)
	{
		Server *server = Server::getServer();
		channelName = message->GetChannelName(args[i]);

		Channel &channel = server->getChannel(channelName);
		Server::getServer()->PartMessage(sender,channelName);
		channel.LeaveMember(sender.getSocket());
		if(channel.getMemberCount() == 0)
			server->removeChannel(channelName);
	}
}
void CommandHandler::execute_kick(Client &sender, const std::vector<std::string> &arguments)
{
	validate_kick(sender, arguments);
	// validate(sender,arguments);
	Server *server = Server::getServer();
	ClientManager *clientManager = ClientManager::getManager();
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Channel &channel = server->getChannel(channelName);
	server->KickMessage(clientManager->getClient(arguments[1]),channelName,sender.getNick());
	int	memberSocket = clientManager->GetClientSocket(arguments[1]);
	channel.KickMember(sender.getSocket(), memberSocket);
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
			server->ChannelModeMessage(sender, channelName);
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
	else
		server->UserModeMessage(sender);
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
		throw IRCException(sender.getNick(), " :You have not registered", 451); //NotRegistered(sender.getNick());
	//validation !!!
	std::ifstream input(arguments.size() >= 1 ? arguments[0] : defaultFileName);

	if (!input) 
	{
		Server::getServer()->SendMessageToClient(sender, "Cannot Open the given File");
		return ;
	}

	std::string line;
	std::string mem;
	while (std::getline(input, line)) 
	{
		mem +=line;
	}
	Server::getServer()->SendMessageToClient(sender, mem);
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
		throw IRCException(sender.getNick(), " " + data.command + " :Unknown command", 421);//UnknownCommand(sender.getNick(), data.command);
	}
}
