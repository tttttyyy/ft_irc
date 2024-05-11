#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <exception>
# include <iostream>
# include "Server.hpp"
# include <sstream>

class Server;

template <typename T>
std::string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

class IRCException : public std::exception
{
protected:
	int errorCode;
	std::string name;
	std::string errorMessage;

private:
	mutable std::string fullMessage;

public:
	IRCException(const std::string &_name, const std::string &_message, int _errorCode);

public:
	virtual const char *what() const throw();
	virtual ~IRCException() throw();
};

#endif // EXCEPTIONS_HPP
