#include "Exceptions.hpp"

IRCException::IRCException(const std::string &_name, const std::string &_message, int _errorCode): errorCode(_errorCode), name(_name), errorMessage(_message)
{}

const char * IRCException::what() const throw()
{
		fullMessage = ":" + name + "@" + (Server::getServer())->getHost() + " " + NumberToString(errorCode) + errorMessage;
		return fullMessage.c_str();
}

IRCException::~IRCException() throw() {}