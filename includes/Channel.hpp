#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Client;
class CommandResponse;

struct ModeType
{
	enum Mode
	{
		none = 0, 
		read = 1,
		write_ = 2,
		invite = 4,
		private_ = 8,
		topic = 16,
		key = 32,
		operator_ = 64,
		user_limit = 128
	};
};


class Channel 
{
	private:
		std::string	name;
		std::string	topic;
		std::string	password;
		std::map<int, Client> members;
		mutable std::vector<int> admins;
		int			mode;
		size_t		limit;

	public:
		
		Channel();
		~Channel();
		Channel(std::string const &_name);
		void	AddMember(int newMember);
		void	SetLimit(size_t new_limit);
		int		size() const;
		void	KickMember(int admin, int removingMember);
		void	LeaveMember(int memberNick);
		void	LeaveIfMember(int memberNick);


		void	MakeAdmin(int admin, int newAdmin);
		void	RemoveFromAdmins(int admin, int oldAdmin);

		bool	IsAdmin(int memberNick) const;
		bool	HasMember(int memberName) const;

		void	Broadcast(const Client &sender, const std::string &message, const std::string &command) const;
		void	SendChannelReply(const std::string &message) const;
		void	SendJoinReply(const Client &client) const;
		void	SendWhoReply(const Client &client) const;
		int		getMemberCount();
		void	PrintData();

		void	SetPassword(const std::string &_password);
		bool	CheckPassword(const std::string &_checkingPass) const;

		//topic
		std::string getTopic(void) const;
		void	setTopic(const std::string t);

		int		HasMode(ModeType::Mode _mode)const;
		void	AddMode(ModeType::Mode mode);
		void	RemoveMode(ModeType::Mode mode);
		std::string	ModeInfo() const;

		int		GetAdmin();
		std::string	GetNickWithSocket(int socket) const;
		
		void ChannelWhoResponse(Client const &client);
		void ChannelJoinResponse(Client const &client);
		void ChangeChannelUser(Client const &client);


	private:
		void	ValidateAdmin(int admin) const;
		void	ValidateAdminIsInChannel(int admin) const;
		void	ValidateClientIsInServer(int client) const;
		void	ValidateClientIsInChannel(int admin, int client) const;
		void	ValidateCanModifyAdmin(int admin, int newAdmin) const;
		void	SetAdmin(int newAdmin);
		void	DeleteAdmin(int removingAdmin);

};




#endif