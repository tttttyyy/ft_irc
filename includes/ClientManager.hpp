#ifndef CLIENT_MANAGER_HPP
# define CLIENT_MANAGER_HPP

#define CLIENTLIMIT 1000

# include "irc.hpp"

class MessageController;

class ClientManager
{
	private:
		std::map<int, Client>	clientMap; // client socket fd, client/user
		mutable std::map<int, Client>::const_iterator it;
		char buffer[1025];
		unsigned long clientLimit;

	private:
		static ClientManager *instance;
		MessageController	*messageController;

	public:
		ClientManager();
		~ClientManager();
		
		bool	HasClient(int clientSocket) const;
		bool	HasClient(const std::string &clientNick) const;
		int		GetClientSocket(const std::string &clientName) const;
		void	AddClient(int socketFd);
		void	RemoveClient(int socketFd);
		void	RemoveClient(std::map<int, Client>::iterator &iter);
		int		AddClientstToReadFds(fd_set *readfds);
		void	CloseClient(int	clientSocket, const std::string &reason);
		void	HandleInput(fd_set *readfds);
		void	HandleMessage(Client &client);

		static ClientManager *getManager();
		const Client &getClient(int clientSocket) const;
		const Client &getClient(const std::string &clientSocket) const;
};


#endif
