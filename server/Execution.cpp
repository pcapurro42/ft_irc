/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execution.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:31:34 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 19:02:33 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::sendError(const char *command, int id, int value) // se charge de communiquer au client l'erreur selon le ERR_ que tu vas return dans tes parsers/executions
{
    string  message;

    if (value == ERR_INVALIDCOMMAND)
    {
        cout << getTime() << "Error! " << _clients_data[id - 1].nickname << " failed to request (invalid or unsupported command)." << endl;
        message = "'" + string(command) + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_UNKNOWNCOMMAND)
    {
        message = "'" + string(command) + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYPARAMS || value == ERR_NEEDMOREPARAMS || value == ERR_NONICKNAMEGIVEN || value == ERR_KEYSET)
    {
        message = "'" + string(command) + "' :Invalid number of parameters.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ERRONEUSNICKNAME)
    {
        message = "'" + string(command) + "' :Invalid nickname.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHNICK)
    {
        message = "'" + string(command) + "' :Member not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NICKCOLLISION)
    {
        message = "'" + string(command) + command + "' :Nickname already taken.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHCHANNEL)
    {
        message = "'" + string(command) + command + "' :Channel not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANNELISFULL)
    {
        message = "'" + string(command) + command + "' :Channel is full.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVITEONLYCHAN)
    {
        message = "'" + string(command) + "' :Invitation required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTONCHANNEL)
    {
        message = "'" + string(command) + "' :Presence in channel required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANOPRIVSNEEDED)
    {
        message = "'" + string(command) + "' :Privileges required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_PASSWDMISMATCH)
    {
        message = "'" + string(command) + "' :Wrong password.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTREGISTERED)
    {
        message = "'" + string(command) + "' :Authentication required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTIDENTIFIED)
    {
        message = "'" + string(command) + "' :Identification required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYREGISTRED)
    {
        message = "'" + string(command) + "' :Authentication already done.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYINCHANNEL)
    {
        message = "'" + string(command) + "' :Channel already joined.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOPRIVILEGES)
    {
        message = "'" + string(command) + "' :No privileges.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
}

void    Server::sendToEveryone(string message, int id, bool self)
{
    for (int i = 0; i != MAX_CLIENTS; i++)
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

    // cout << "Command received from " << _clients_data[id - 1].nickname << " : '" << command << "'." << endl;

    if (cmd_name == "PING")
        value = executePingCommand(cmd, id - 1);
    else if (cmd_name == "PONG")
        value = executePongCommand(cmd, id - 1);
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
    else if (cmd_name == "WHO")
        value = executeWhoCommand(cmd, id - 1);
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

1. J'ai laissé PASS, USER, NICK, QUIT et PING comme modèles pour que tu puisses comprendre
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
- MAX_CANALS // nombre max de channels dans le tableau

5. Au sein de ces fonctions, tu peux utiliser :
- searchCanal(string nom_du_canal)
=> renvoie -1 si le canal n'existe pas ou renvoie l'index du canal dans _canals[] si il existe
- searchClient(string nickname_du_client)
=> renvoie -1 si le client n'existe pas ou renvoie l'index du client dans _clients_data[] si il existe
- getArgument(string, numéro de l'argument)
=> si par exemple tu as ta string qui contient la commande 'KICK #canal membre' et que tu fais getArgument(cmd, 2), ça te retournera une string qui contient 'membre', et si tu fais getArgument(cmd, 6) sur cette même commande, ça te renverra une string vide ("") pour t'indiquer que l'argument n'existe pas (ça te servira pour le parser)

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

Commandes à réaliser:
- INVITE
- JOIN
- KICK
- MODE
- PART
- PRIVMSG #canal <message>
- PRIVMSG utilisateur <message>
- TOPIC

voir RFC 1459

NOTE 4:

Quelques contraintes qui seront pas précisées dans la doc et que tu vas devoir implémenter:
1. Pour exécuter n'importe quelle commande à part PING, il faut être authentifié (authentified dans chaque case _clients_data)
-> Il faut avoir exécuté 'PASS [mdp]' avec le bon mot de passe, sinon return directement ERR_NOTREGISTERED
2. Après avoir validé le mot de passe, pour exécuter n'importe quelle commande à part PASS, PING, USER et NICK, il faut avoir utilisé USER ET NICK (= s'être identifié, variable identified dans chaque case _clients_data, strings username, realname et nickname)

NOTE 5:

Je m'occuperai de toute la partie communication server-client dans tes fonctions
Par exemple: prenons la fonction KICK qui cible un utilisateur
- Tu dois parser si la commande est valide dans sa syntaxe (voir rfc 1459)
- Tu dois vérifier si la commande est réalisable (= que l'utilisateur qui l'exécute s'est authentifié (variable authentified) et s'est identifié (identified))
- Tu dois vérifier si la commande est cohérente (= le canal existe, l'utilisateur ciblé existe et se trouve dans le canal, l'utilisateur envoyeur possède les droits pour kick la cible (= il est présent dans le vecteur operators du canal), etc)
- SI tu n'as rencontré aucune ERR_ sur le chemin, tu dois exécuter la commande (= mettre à jour la liste des membres dans le vecteur du canal seulement pour cette commande)
- Enfin le retour client (utiliser send() pour prévenir le client ciblé qu'il a été kick + pour prévenir les autres membres du canal qu'il a été kick) tu pourras laisser un commentaire dans le code pour préciser ou le faire et je m'en occuperai

*/