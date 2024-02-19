/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:47:40 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:41:55 by pcapurro         ###   ########.fr       */
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
# define ERR_NOPRIVILEGES 440
# define ERR_ALREADYREGISTRED 462

# define SLOTS_LIMIT 21
# define CANALS_LIMIT 42

typedef struct s_canal
{
    string          name;

    bool            op_topic;
    string          topic;

    vector<string>  invited;
    bool            invite_only;

    bool            pass_only;
    string          password;

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

        void            verifyPing(void);

        int             addSocket(int socket);
        void            removeSocket(int id);
        void            addClient(void);
        void            removeClient(int id, int value);
        void            receiveData(int id);

        void            startLoopRoutine(void);
        void            initializeServer(void);

        bool            isMemberOfCanal(string clientname);
        bool            isOperatorOfCanal(string clientname);
        int             searchCanal(string canal_name) const;
        int             searchClient(string clientname) const;

        int             executePingCommand(string cmd, int id);
        int             executeNickCommand(string cmd, int id);
        int             executeUserCommand(string cmd, int id);
        int             executeQuitCommand(string cmd, int id);
        int             executePassCommand(string cmd, int id);
        int             executeJoinCommand(string cmd, int id);
        int             executePartCommand(string cmd, int id);
        int             executeKickCommand(string cmd, int id);
        int             executeInviteCommand(string cmd, int id);
        int             executePrivmsgCommand(string cmd, int id);
        int             executeTopicCommand(string cmd, int id);
        int             executeModeCommand(string cmd, int id);

        void            executeCommand(const char *command, string cmd_name, int id);
        void            sendToEveryone(string message, int id, bool self);
        void            sendError(const char *command, int id, int value);

    private:
        int             _port;
        string          _password;

        int             _clients_slots;
        int             _canals_slots;
        int             _clients_nb;
        int             _canals_nb;

        t_client_data   _clients_data[SLOTS_LIMIT];
        t_canal         _canals[CANALS_LIMIT];

        struct pollfd   _sockets_array[SLOTS_LIMIT + 1];
        int             _server_socket;

        int             _fail;
};

#endif