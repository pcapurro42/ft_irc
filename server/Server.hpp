/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:47:40 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/24 11:53:47 by ory              ###   ########.fr       */
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

# ifndef MAX_CLIENTS
#  define MAX_CLIENTS 0
# endif

# ifndef MAX_CANALS
#  define MAX_CANALS 0
# endif

typedef struct s_canal
{
    string          name;       // nom du canal

    bool            op_topic;   // est-ce qu'il faut être admin pour changer le topic
    string          topic;      // sujet du canal

    vector<string>  invited;    // liste des invités (!= des membres présents)
    bool            invite_only;// est-ce qu'une invitation (être dans la liste de invited) est requise

    bool            pass_only;  // est-ce qu'un mot de passe est requis pour se connecter
    string          password;   // mot de passe pour entrer dans le canal (argument de JOIN)

    string          last_message; // variable qui contient les derniers messages des utilisateurs dans le canal

    vector<string>  members;    // list des personnes présentes dans le salon (qui reçoivent donc les messages)
    vector<string>  operators;  // liste des admins du canal

    int             max;        // nb de places maximum
    bool            exist;      // est-ce que le canal a été créé

}   t_canal;

typedef struct s_client_data
{
    string  nickname;       // seul argument de NICK
    string  username;       // premier argument de USER
    string  realname;       // quatrième argument de USER

    int     number;         // numéro d'identification unique attribué à chaque utilisateur

    bool    authentified;   // est-ce que l'utilisateur a envoyé le mot de passe (deuxième étape)
    bool    identified;     // est-ce que l'utilisateur s'est présenté (en utilisant USER ET NICK) (troisième étape)
    bool    connected;      // est-ce que l'utilisateur s'est juste connecté au serveur (première étape)

    bool    ping;
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

        bool            fail(void) const;   // est-ce que la construction du serveur a échoué
        bool            end(void) const;    // fonction qui prévient qu'il faut arrêter le serveur (pour quitter la boucle)
        void            setFail();          // fonction qui prévient que la construction a échoué

        bool            validateInput(const int port_nb, const char *password) const; // parser d'input du programme de base

        string          getArgument(const string cmd, int nb_arg) const;    // voir le fichier de la fonction
        string          convertNumberToString(int value) const;             // itoa version cpp

        void            sendPing(void);
        void            verifyTimeOut(void);   // fonction que j'écrirai plus tard

        int             addSocket(int socket);  // ajoute un socket client au tableau des sockets connectés (_sockets_array[])
        void            removeSocket(int id);
        void            addClient(void);        // appelle addSocket() + créé le profil dans _clients_data[]
        void            removeClient(int id);
        void            receiveData(int id);    // récupère la string sur le socket client qui vient d'émettre

        vector<string>  rectifyInput(string buffer) const;

        void            startLoopRoutine(void); // routine principale du serveur
        void            initializeServer(void);

        int             searchCanal(string canal_name) const;   // renvoi l'index du canal dans _canals[] ou -1 si il n'existe pas
        int             searchClient(string clientname) const;  // renvoie l'index du client dans _clients_data[] ou -1 si il n'existe pas

        int             executePingCommand(string cmd, int id);
        int             executePongCommand(string cmd, int id);
        int             executeNickCommand(string cmd, int id);
        int             executeUserCommand(string cmd, int id);
        int             executeQuitCommand(string cmd, int id);
        int             executePassCommand(string cmd, int id);
        int             executeJoinCommand(string cmd, int id);
        int             executeWhoCommand(string cmd, int id);
        int             executePartCommand(string cmd, int id);
        int             executeKickCommand(string cmd, int id);
        int             executeInviteCommand(string cmd, int id);
        int             executePrivmsgCommand(string cmd, int id);
        int             executeTopicCommand(string cmd, int id);
        int             executeModeCommand(string cmd, int id);

        void            executeCommand(string command, string cmd_name, int id);
        void            sendToEveryone(string message, int id, bool self);
        void            sendError(const char *command, int id, int value);

        void            createCanal(const std::string& channels, const std::string& nickname);

        std::string     getTime(void) const;

    private:
        int             _port;      // port du serveur
        string          _password;  // mot de passe du serveur

        int             _clients_slots; // nombre de clients connectés (s'incrémente/se décrémente à la connexion/déconnexion)
        int             _clients_nb; // nombre de connexions depuis l'existence du serveur (ne se décrémente pas à la déconnexion)

        t_client_data   _clients_data[MAX_CLIENTS]; // tableau contenant toutes les informations de tous les clients (string vide si la case n'est pas allouée pour un client)
        t_canal         _canals[MAX_CANALS]; // tableau contenant toutes les informations de tous les channels

        struct pollfd   _sockets_array[MAX_CLIENTS + 1];    // tableau contenant tous les sockets de tous les clients connectés (= point d'accès par lequel renvoyer une réponse) + le socket du serveur
        int             _server_socket; // socket du serveur (= case 0 du tableau sockets_array[])

        int             _fail;  // variable pour savoir si la construction du serveur a échoué
};

#endif