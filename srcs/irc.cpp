
#include "irc.hpp"

void	SendMessageWithSocket(int clientSocket,	const std::string &message){
	if (send(clientSocket, (message + "\n").c_str(), message.length() + 2, 0) < 0)
		perror("send");
}

void    SendMessageToClient(const Client &client, const std::string &message){
	SendMessageWithSocket(client.getSocket(), message);
}

void    PartMessage(const Client &client,std::string const &channelName){
    (void) client;
	Channel &channel = Server::getServer()->getChannel(channelName);
	std::string part_reply = client.GetFormattedText() + " PART " + "#" + channelName;
    channel.SendChannelReply(part_reply);
}

void    KickMessage(const Client &client, std::string const &channelName, std::string const &admin){
    (void) client;
	Channel &channel = Server::getServer()->getChannel(channelName);
	const Client &admin_class = ClientManager::getManager()->getClient(admin);
	std::string message = ":" + admin_class.GetFormattedText() + " KICK " + "#"+channelName + " " + client.getNick() + " : BYE!!!";
	std::string kick_reply = ":KICK FROM #" + channelName + " BY " + admin;
    channel.SendChannelReply(message);
}

void    UserModeMessage(const Client &client){
	std::string endingString = "221  ";
	SendMessageToClient(client, endingString);
}

void    ChannelModeMessage(const Client &client, const std::string &channelName){
	Channel &channel = Server::getServer()->getChannel(channelName);
	std::string endingString = "324 " + client.getNick() + " #"
		+ channelName + " " + channel.ModeInfo();
	SendMessageToClient(client, endingString);
}

int custom_stoi(const std::string& str) {
    int result = 0;
    bool negative = false;
    size_t i = 0;

    if (str[i] == '-') {
        negative = true;
        ++i;
    } else if (str[i] == '+') {
        ++i;
    }

    for (; i < str.size(); ++i) {
        if (!isdigit(str[i])) {
            break;
        }
        result = result * 10 + (str[i] - '0');
    }

    if (negative) {
        result = -result;
    }

    return result;
}
