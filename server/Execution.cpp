/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execution.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:31:34 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 23:50:36 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::sendError(const char *command, int id, int value) // se charge de communiquer au client l'erreur selon le ERR_ que tu vas return dans tes parsers/executions
{
    string  message;

    if (value == ERR_INVALIDCOMMAND)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " failed to request (invalid or unsupported command)." << endl;
        message = "421 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_UNKNOWNCOMMAND)
    {
        message = "421 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYPARAMS || value == ERR_NEEDMOREPARAMS || value == ERR_NONICKNAMEGIVEN || value == ERR_KEYSET)
    {
        message = "461 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid number of parameters.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ERRONEUSNICKNAME)
    {
        message = "433 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid nickname.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHNICK)
    {
        message = "401 " + _clients_data[id - 1].nickname + " '" + command + "' :Member not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NICKCOLLISION)
    {
        message = "436 " + _clients_data[id - 1].nickname + " '" + command + "' :Nickname already taken.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHCHANNEL)
    {
        message = "403 " + _clients_data[id - 1].nickname + " '" + command + "' :Channel not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANNELISFULL)
    {
        message = "471 " + _clients_data[id - 1].nickname + " '" + command + "' :Channel is full.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVITEONLYCHAN)
    {
        message = "474 " + _clients_data[id - 1].nickname + " '" + command + "' :Invitation required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTONCHANNEL)
    {
        message = "442 " + _clients_data[id - 1].nickname + " '" + command + "' :Presence in channel required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANOPRIVSNEEDED)
    {
        message = "482 " + _clients_data[id - 1].nickname + " '" + command + "' :Privileges required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_PASSWDMISMATCH)
    {
        message = "161 " + _clients_data[id - 1].nickname + " '" + command + "' :Wrong password.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTREGISTERED)
    {
        message = "451 " + _clients_data[id - 1].nickname + " '" + command + "' :Authentication required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYREGISTRED)
    {
        message = "462 " + _clients_data[id - 1].nickname + " '" + command + "' :Authentication already done.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYINCHANNEL)
    {
        message = "477 " + _clients_data[id - 1].nickname + " '" + command + "' :Channel already joined.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOPRIVILEGES)
    {
        message = "440 " + _clients_data[id - 1].nickname + " '" + command + "' :No privileges.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
}

void    Server::sendToEveryone(string message, int id, bool self)
{
    for (int i = 0; i != SLOTS_LIMIT; i++)
    {
        if (self == true)
            send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
        else
        {
            if (i != id)
                send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
        }
    }
}

void    Server::executeCommand(const char *command, string cmd_name, int id)
{
    int value = ERR_INVALIDCOMMAND;
    string cmd(command);

    cout << "Command received from " << _clients_data[id - 1].nickname << " : '" << command << "'." << endl;

    if (cmd_name == "PING")
        value = executePingCommand(cmd, id - 1);
    else if (cmd_name == "PASS")
        value = executePassCommand(cmd, id - 1);
    else if (cmd_name == "NICK")
        value = executeNickCommand(cmd, id - 1);
    else if (cmd_name == "USER")
        value = executeUserCommand(cmd, id - 1);
    else if (cmd_name == "QUIT")
        value = executeQuitCommand(cmd, id - 1);
    else if (cmd_name == "JOIN")
        value = executeJoinCommand(cmd, id - 1);
    else if (cmd_name == "PART")
        value = executePartCommand(cmd, id - 1);
    else if (cmd_name == "PRIVMSG")
        value = executePrivmsgCommand(cmd, id - 1);
    else if (cmd_name == "KICK")
        value = executeKickCommand(cmd, id - 1);
    else if (cmd_name == "INVITE")
        value = executeInviteCommand(cmd, id - 1);
    else if (cmd_name == "TOPIC")
        value = executeTopicCommand(cmd, id - 1);
    else if (cmd_name == "MODE")
        value = executeModeCommand(cmd, id - 1);

    if (value != 0)
        sendError(command, id, value);
}

/*

NOTE:

1. J'ai laissé PASS, QUIT et PING comme modèles pour que tu puisses comprendre
comment correctement exécuter une fonction tout en interagissant avec les données du serveur

2. en argument de toutes les commandes sont passées :
- la commande elle-même (en string)
- l'id de la personne qui a exécuté cette commande
=> ainsi tu peux accéder par exemple au nom de la personne qui a envoyé la commande au serveur en faisant _clients_data[id].nickname

3. Dans chaque fonction execute, tu dois renvoyer une valeur ERR_ en fonction de ton parser, en revanche si la commande est correcte
ET qu'elle peut s'exécuter (possession des droits, cohérence etc), tu dois renvoyer 0

4. Au sein de ces fonctions, tu n'as pas besoin de manipuler autre chose que :
- _clients_data[] // une case du tableau = une structure contenant les informations d'un utilisateur
- _canals[] // une case du tableau = une structure contenant les informations d'un channel
- _clients_slots (int) // juste le nombre total de clients connectés depuis le démarrage (valeur s'incrémentant à chaque connexion
et se décrémentant à chaque déconnexion, tu n'as pas à gérer ça c'est déjà fait)
- _canals_slots (int) // juste le nombre de channels créés/actifs
- CANALS_LIMIT // nombre max de channels dans le tableau

Important : le profil socket d'un client est à i + 1 du profil qui contient les données
Exemple : si l'utilisateur à ses infos à _clients_data[i], son socket est toujours à _sockets_array[i + 1] (décalage à cause de la présence du socket serveur à _sockets_array[0])
Pouvoir connaitre l'emplacement du socket d'un client te servira lorsque tu renverras un message
Par exemple: l'utilisateur envoie PING 'texte', il faut lui répondre PONG 'texte', donc il faut envoyer (send()) "PONG 'text'" au socket correspondant, je réexpliquerai en vocal si besoin

NOTE 2:

Pour tester le programme, lancer le ./ircserv avec comme argument 6667 et un mot de passe random
Puis lancer le client Konversation, ajouter un serveur et renseigner 127.0.0.1 comme ip et 6667 comme port
Ce qu'il va se passer (tu pourras l'observer avec le print que j'ai mis à la ligne 121) quand le client va se connecter :
1. Il envoie "CAP LS 302" (commande à ignorer, à ne pas traiter, elle sert à demander au serveur les capacités)
2. Il envoie ensuite "USER [username] [valeur à ignorer] [valeur à ignorer] [realname]
3. Il envoie ensuite "NICK [nickname]"
4. Par la suite il envoie régulièrement un PING (cela sera traiter automatiquement car j'ai laissé la commande PING)
5. Puis il essaie de temps en temps HOST et une autre commande je sais plus qui sont à IGNORER et qui produiront un message dans le log du serveur (Error unkown command tu verras)
Pour tester les commandes, simplement écrire dans la ligne dédiée la commande en brute en majuscule (ex: QUIT "message")

NOTE 3:

Quelques contraintes qui seront pas précisées dans la doc et que tu vas devoir implémenter:
1. Pour exécuter n'importe quelle commande à part PING, il faut être authentifié (authentified dans chaque case _clients_data)
-> Il faut avoir exécuté 'PASS [mdp]' avec le bon mot de passe, sinon return directement ERR_NOTREGISTERED
2. Après avoir validé le mot de passe, pour exécuter n'importe quelle commande à part PASS, PING, USER et NICK, il faut avoir utilisé USER ET NICK (= s'être identifié, variable identified dans chaque case _clients_data, strings username, realname et nickname)

*/