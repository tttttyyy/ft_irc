#pragma once

// Numeric replies 
#define RPL_WELCOME(source)                     "001 " + source + " :Welcome to the ft_irc Network"
#define RPL_ENDOFWHO(name, source)              "315 " + name + " " + source + " :End of WHO list"
#define RPL_CHANNELMODEIS(channelName1, channelName2, mode_params) "324 " + channelName1 + " " + channelName2 + " " + " " + mode_params
#define RPL_NOTOPIC(channelName)                "331 # " + channelName
#define RPL_TOPIC(channelName, topic)           "332 # " + channelName + " :" + topic
#define RPL_INVITING(name, nick, ch)            "341 " + name + " " + nick + " " + ch
#define RPL_WHOREPLY(name, ch, us, hst, nick, fl, re)"352 " + name + " " + ch + " " + us + " " + hst + " ft_irc " + nick + " " + fl + " :" + re
#define RPL_NAMREPLY(nick, ch, nickList)        "353 " + nick + " = " + ch + " :" + nickList
#define RPL_ENDOFNAMES(name, ch)                "366 " + name +  " " + ch + " :End of /NAMES list"

// Error replies 
#define ERR_NOSUCHNICK(source, command)         "401 " + source + " " + command + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(source, command)      "403 " + source + " " + command + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(source, command)   "404 " + source + " " + command + " :Cannot send to channel"
#define ERR_NORECIPIENT(source, command)        "411 " + source + " :No recipient given (" + command + ")"
#define ERR_NOTEXTTOSEND(source)                "412 " + source + " :No text to send"
#define ERR_UNKNOWNCOMMAND(source, command)     "421 " + source + " " + command + " :Unknown command"
#define ERR_NONICKNAMEGIVEN(source)             "431 " + source + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(source, command)   "432 " + source + " " + command + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source, command)      "433 " + source + " " + command + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL(src, com1, com2)   "441 " + src + " " + com1 + " " + com2 + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(source, command)       "442 " + source + " " + command + " :You're not on that channel"
#define ERR_USERONCHANNEL(src, com1, com2)      "443 " + src + " " + com1 + " " + com2 + " :is already on channel"
#define ERR_NOTREGISTERED(source)               "451 " + source + " :You have not registered"
#define ERR_NEEDMOREPARAMS(source, command)     "461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(source)           "462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)               source + " :Password incorrect" //464
#define ERR_KEYSET(source, command)             "467 " + source + " " + command + " :Channel key already set"
#define ERR_CHANNELISFULL(source, command)      "471 " + source + " " + command + " :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE(source, command, msg)   "472 " + source + " " + command + " " + msg
#define ERR_INVITEONLYCHAN(source, command)     "473 " + source + " " + command + " :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY(source, command, msg) "475 " + source + " " + command + " " + msg
#define ERR_NOCHANMODES(channelName)            "477 # " + channelName + " :TOPIC mode is OFF ('MODE <channel> +t' is needed)"
#define ERR_BADCHANMASK(source, command)        "476 " + source + " " + command + " :Bad Channel Mask"
#define ERR_CHANOPRIVSNEEDED(source, command)   "482 " + source + " " + command + " :You're not channel operator"

#define RPL_MSG(prefix, command, target, massage)   ":" + prefix + " " + command + " " + target + " :" + massage
#define RPL_KICK(prefix, channel, target, reason)   ":" + prefix + " KICK " + channel + " " + target + " :" + reason
#define RPL_PING(prefix, massage)                   ":" + prefix + " PONG :" + massage
#define RPL_QUIT(prefix, massage)                   ":" + prefix + " QUIT :Quit: " + massage
#define RPL_JOIN(prefix, target)                    ":" + prefix + " JOIN " + target
#define RPL_PART(prefix, target)                    ":" + prefix + " PART " + target
#define RPL_MODE(prefix, channel, target)           ":" + prefix + " MODE " + channel + " " + target
#define RPL_INVITE(prefix, target, channel)         ":" + prefix + " INVITE " + target + " " + channel
