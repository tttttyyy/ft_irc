#include "Exceptions.hpp"

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
	IRCException(const std::string &_name, const std::string &_message, int _errorCode)
		: errorCode(_errorCode), name(_name), errorMessage(_message) {}

public:
	virtual const char *what() const throw(){
		//: name@host ERR_CODE name:reason
		fullMessage = ":" + name + "@" + (Server::getServer())->getHost() + " " + NumberToString(errorCode) + errorMessage;
		return fullMessage.c_str();
	}
	virtual ~IRCException() throw() {}
};

#endif // EXCEPTIONS_HPP
