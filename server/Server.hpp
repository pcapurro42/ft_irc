/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:47:40 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 17:40:58 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER
# define SERVER

# include "../Irc.hpp"

# define ERR_INVALIDCOMMAND 460
# define ERR_UNKNOWNCOMMAND 421

# define ERR_TOOMANYPARAMS 461
# define ERR_NEEDMOREPARAMS 461
# define ERR_ERRONEUSNICKNAME 433
# define ERR_NONICKNAMEGIVEN 431
# define ERR_NOSUCHNICK 401
# define ERR_NICKCOLLISION 436
# define ERR_NOSUCHCHANNEL 403
# define ERR_CHANNELISFULL 471
# define ERR_BADCHANNELKEY 475
# define ERR_INVITEONLYCHAN 474
# define ERR_NOTONCHANNEL 442
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_KEYSET 467
# define ERR_ALREADYINCHANNEL 477
# define ERR_ALREADYINVITED 478
# define ERR_PASSWDMISMATCH 161
# define ERR_NOTREGISTERED 451
# define ERR_NOTIDENTIFIED 452
# define ERR_NOPRIVILEGES 440
# define ERR_ALREADYREGISTRED 462
# define ERR_TOOMANYCHANNELS 405
# define ERR_INVALIDCHANNELNAME 470
# define ERR_TOOMANYMODEFLAGS 472
# define ERR_CANNOTSENDTOCHAN 404
# define ERR_INACTIVEBOT 406

# ifndef MAX_CLIENTS
#  define MAX_CLIENTS 42
# endif

# ifndef MAX_CANALS
#  define MAX_CANALS 21
# endif

# ifndef BOT
#  define BOT 0
# endif

typedef struct s_canal
{
    std::string          name;

    bool            op_topic;
    std::string          topic;

    std::vector<std::string>  invited;
    bool            invite_only;

    bool            pass_only;
    std::string          password;

    std::vector<std::string>  members;
    std::vector<std::string>  operators;

    int             max;
    bool            exist;

}   t_canal;

typedef struct s_client_data
{
    std::string  nickname;
    std::string  username;
    std::string  realname;

    int     number;

    bool    authentified;
    bool    identified;
    bool    connected;

    bool    ping;
    std::string  last_command;

    bool    set_nickname;
    bool    set_username;

}   t_client_data;

class Server
{
    public:
        Server(const int port, const char *password);
        ~Server();

        Server(const Server &original);
        Server  &operator=(const Server &original);

        bool            fail(void) const;
        bool            validateInput(const int port_nb, const char *password) const;

        bool            end(void) const;
        void            setFail();

        void            LoopRoutine(void);

    private:
        void            initializeServer(void);

        void            removeEmptyChannels(void);
        void            sendPing(void);
        void            verifyTimeOut(void);

        int             addSocket(int socket);
        void            removeSocket(int id);
        void            addClient(void);
        void            removeClient(int id);
        void            receiveData(int id);

        std::string     getTime(void) const;
        std::string     getMessage(std::string cmd) const;

        void            executeCommand(std::string command, std::string cmd_name, int id);
        void            sendToEveryone(std::string message, int id, bool self);
        void            sendToEveryChannelMembers(std::string message, std::string channel);
        void            sendError(const char *command, int id, int value);

        std::vector<std::string>    rectifyInput(std::string buffer) const;
        std::vector<std::string>    splitStringToVector(std::string str) const;
        
        std::string     getArgument(const std::string cmd, int nb_arg) const;
        std::string     convertNumberToString(int value) const;

        int             searchCanal(std::string canal_name) const;
        int             searchClient(std::string clientname) const;

        bool            isAuthentified(std::string clientname) const;
        bool            isIdentified(std::string clientname) const;

        int             executePingCommand(std::string cmd, int id);
        int             executePongCommand(std::string cmd, int id);
        
        int             executeNickCommand(std::string cmd, int id);
        int             executeUserCommand(std::string cmd, int id);
        
        int             executeQuitCommand(std::string cmd, int id);
        
        int             executePassCommand(std::string cmd, int id);
        
        int             executeJoinCommand(std::string cmd, int id);
        
        int             executeWhoCommand(std::string cmd, int id);
        
        int             executePartCommand(std::string cmd, int id);
        
        int             verifyKickCMD(std::string cmd, int id) const;
        int             executeKickCommand(std::string cmd, int id);
        
        int             verifyInviteCMD(std::string cmd, int id) const;
        int             executeInviteCommand(std::string cmd, int id);
        
        int             executePrivmsgCommand(std::string cmd, int id);
        
        int             executeTopicCommand(std::string cmd, int id);
        
        int             verifyModeCMD(std::string cmd, int id) const;
        std::string     processModeCMD(std::string cmd) const;
        void            executeModeICommand(int canal_id, int id, char sign);
        void            executeModeTCommand(int canal_id, int id, char sign);
        void            executeModeKCommand(std::string password, int canal_id, int id, char sign);
        void            executeModeOCommand(std::string member, int canal_id, int id, char sign);
        void            executeModeLCommand(int value, int canal_id, int id);
        int             executeModeCommand(std::string cmd, int id);
        
        int             executeBotCommand(std::string cmd, int id);
        void            botTOTD(int id, std::string msg);

        void            createCanal(const std::string channels, const std::string nickname);

        int             _port;
        std::string          _password;

        int             _clients_slots;
        int             _clients_nb;

        t_client_data   _clients_data[MAX_CLIENTS];
        t_canal         _canals[MAX_CANALS];

        struct pollfd   _sockets_array[MAX_CLIENTS + 1];
        int             _server_socket;

        int             _fail;
};

#endif