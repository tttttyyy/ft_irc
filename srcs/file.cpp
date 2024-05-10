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

 
template<>
void	Command<CommandType::part>::validate(Client &sender,const std::vector<std::string> &arguments)

 
template<>
void	Command<CommandType::kick>::validate(Client &sender,const std::vector<std::string> &arguments)


void	ValidateChannelMode(const Client &sender, const std::vector<std::string> &arguments)


template<>
void	Command<CommandType::mode>::validate(Client &sender,const std::vector<std::string> &arguments)


template <>
void	Command<CommandType::who>::validate(Client &sender, const std::vector<std::string> &arguments)


template<>
void	Command<CommandType::bot>::validate(Client &sender, const std::vector<std::string> &arguments)
