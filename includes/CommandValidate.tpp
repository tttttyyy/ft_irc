template <CommandType::Type type>
void	Command<type>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;

}

template<>
void	Command<CommandType::pass>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed())
		throw IRCException(sender.getNick(), " :You may not reregister", 462);//AlreadyRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw  IRCException(sender.getNick(), " PASS :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"PASS");
	if (Server::getServer()->getPass() != arguments[0])
		throw IRCException(sender.getNick(), " :Password incorrect", 464);//PasswordMissmatch(sender.getNick());
}

template<>
void	Command<CommandType::user>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() < 4)
		throw IRCException(sender.getNick(), " USER :Not enough parameters", 461); // NeedMoreParams(sender.getNick(),"USER");
	if(sender.getIsUsered())
		throw IRCException(sender.getNick(), " :You may not reregister", 462); //AlreadyRegistered(sender.getNick());
}

template<>
void	Command<CommandType::nick>::validate(Client &sender,const std::vector<std::string> &arguments)
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

template<>
void	Command<CommandType::ping>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PING :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"PING");
}
 
template<>
void	Command<CommandType::pong>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw IRCException(sender.getNick(), " :You have not registered", 451) ; //NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw IRCException(sender.getNick(), " PONG :Not enough parameters", 461); //NeedMoreParams(sender.getNick(),"PONG");
}
 
template<>
void	Command<CommandType::privmsg>::validate(Client &sender,const std::vector<std::string> &arguments)
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
 
template<>
void	Command<CommandType::join>::validate(Client &sender,const std::vector<std::string> &arguments)
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
 
template<>
void	Command<CommandType::part>::validate(Client &sender,const std::vector<std::string> &arguments)
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
 
template<>
void	Command<CommandType::kick>::validate(Client &sender,const std::vector<std::string> &arguments)
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

template<>
void	Command<CommandType::mode>::validate(Client &sender,const std::vector<std::string> &arguments)
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

template <>
void	Command<CommandType::who>::validate(Client &sender, const std::vector<std::string> &arguments)
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

template<>
void	Command<CommandType::bot>::validate(Client &sender, const std::vector<std::string> &arguments)
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
