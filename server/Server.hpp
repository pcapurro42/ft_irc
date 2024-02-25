/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:47:40 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 17:09:42 by pcapurro         ###   ########.fr       */
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
# define ERR_PASSWDMISMATCH 161
# define ERR_NOTREGISTERED 451
# define ERR_NOTIDENTIFIED 452
# define ERR_NOPRIVILEGES 440
# define ERR_ALREADYREGISTRED 462
# define ERR_TOOMANYCHANNELS 405
# define ERR_INVALIDCHANNELNAME 470
# define ERR_TOOMANYMODEFLAGS 472
# define ERR_CANNOTSENDTOCHAN 404

# ifndef MAX_CLIENTS
#  define MAX_CLIENTS 0
# endif

# ifndef MAX_CANALS
#  define MAX_CANALS 0
# endif

# ifndef BOT
#  define BOT 0
# endif

typedef struct s_canal
{
    string          name;

    bool            op_topic;
    string          topic;

    vector<string>  invited;
    bool            invite_only;

    bool            pass_only;
    string          password;

    string          last_message;

    vector<string>  members;
    vector<string>  operators;

    int             max;
    bool            exist;

}   t_canal;

typedef struct s_client_data
{
    string  nickname;
    string  username;
    string  realname;

    int     number;

    bool    authentified;
    bool    identified;
    bool    connected;

    bool    ping;
    int     ping_nb;
    string  last_command;

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
        bool            end(void) const;
        void            setFail();

        bool            validateInput(const int port_nb, const char *password) const;

        string          getArgument(const string cmd, int nb_arg) const;
        string          convertNumberToString(int value) const;

        void            sendPing(void);
        void            verifyTimeOut(void);

        int             addSocket(int socket);
        void            removeSocket(int id);
        void            addClient(void);
        void            removeClient(int id);
        void            receiveData(int id);

        vector<string>  rectifyInput(string buffer) const;

        void            startLoopRoutine(void);
        void            initializeServer(void);

        int             searchCanal(string canal_name) const;
        int             searchClient(string clientname) const;

        int             executePingCommand(string cmd, int id);
        int             executePongCommand(string cmd, int id);
        
        int             executeNickCommand(string cmd, int id);
        int             executeUserCommand(string cmd, int id);
        
        int             executeQuitCommand(string cmd, int id);
        
        int             executePassCommand(string cmd, int id);
        
        int             executeJoinCommand(string cmd, int id);
        
        int             executeWhoCommand(string cmd, int id);
        
        int             executePartCommand(string cmd, int id);
        
        int             verifyKickCMD(string cmd, int id) const;
        int             executeKickCommand(string cmd, int id);
        
        int             verifyInviteCMD(string cmd, int id) const;
        int             executeInviteCommand(string cmd, int id);
        
        int             executePrivmsgCommand(string cmd, int id);
        
        int             executeTopicCommand(string cmd, int id);
        
        int             verifyModeCMD(string cmd, int id) const;
        void            executeModeICommand(int canal_id, int id);
        void            executeModeTCommand(int canal_id, int id);
        void            executeModeKCommand(string password, int canal_id, int id);
        void            executeModeOCommand(string member, int canal_id, int id);
        void            executeModeLCommand(int value, int canal_id, int id);
        int             executeModeCommand(string cmd, int id);
        
        int             executeBotCommand(std::string cmd, int id);
        
        void            botTOTD(int id, std::string msg);

        void            executeCommand(string command, string cmd_name, int id);
        void            sendToEveryone(string message, int id, bool self);
        void            sendError(const char *command, int id, int value);

        void            createCanal(const std::string& channels, const std::string& nickname);

        std::string     getTime(void) const;
        std::string     getMessage(std::string cmd);

    private:
        int             _port;
        string          _password;

        int             _clients_slots;
        int             _clients_nb;

        t_client_data   _clients_data[MAX_CLIENTS];
        t_canal         _canals[MAX_CANALS];

        struct pollfd   _sockets_array[MAX_CLIENTS + 1];
        int             _server_socket;

        int             _fail;
};

#endif