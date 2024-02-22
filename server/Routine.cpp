/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:10:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 23:20:37 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool end_;

void    Server::verifyTimeOut(void)
{
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].connected == true)
        {
            if (_clients_data[i].ping == false)
                removeClient(i + 1, 2);
            else
                _clients_data[i].ping = false;
        }
    }
}

void    Server::sendPing(void)
{
    string message;
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        message = "PING :" + _clients_data[i].nickname + "\r\n";
        if (_clients_data[i].connected == true)
            send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
    }
}

void    Server::receiveData(int id)
{
    if (_sockets_array[id].revents == POLLIN)
    {
        char    buffer[512];
        for (int i = 0; i != 512; i++)
            buffer[i] = '\0';

        int value = recv(_sockets_array[id].fd, buffer, 511, 0);
        if (value == -1)
            cerr << "Error! Couldn't receive data from Client #" << _clients_nb + 1 << "." << endl;
        else if (value == 0)
            removeClient(id, 0);
        else
        {
            vector<string>  cmds = rectifyInput(buffer);
            for (vector<string>::iterator k = cmds.begin(); k != cmds.end(); k++)
            {
                string cmd_name = getArgument(*k, 0);
                executeCommand(*k, cmd_name, id);
            }

            // if (std::count(cmd.begin(), cmd.end(), '\n') != 1)
            // {
            //     _clients_data[id - 1].last_command = _clients_data[id - 1].last_command + cmd;
            //     cout << getTime() << "Partial command received from " << _clients_data[id - 1].nickname << ". Conserving it." << endl;
            // }
            // else
            // {
            //     for (int i = 0; cmd[i] != '\0'; i++)
            //     {
            //         if (cmd[i] == '\n' || cmd[i] == '\0')
            //             cmd[i] = '\0';
            //     }
            //     if (_clients_data[id - 1].last_command.empty() == false)
            //     {
            //         cmd = _clients_data[id - 1].last_command + cmd;
            //         _clients_data[id - 1].last_command.clear();
            //     }
            // }
        }
    }
    else if (_sockets_array[id].revents == POLLERR || _sockets_array[id].revents == POLLNVAL) // ici le signal du socket signifie que le client se déconnecte aussi mais pour une erreur
        removeClient(id, 1);
}

void    Server::startLoopRoutine(void)
{
    addSocket(_server_socket); // ajout du socket du serveur (le socket qui ne va servir QU'À réceptionner les nv arrivants)

    _canals[0].name = "#canal1"; // création d'un canal dès la construction pour y faire des tests (à retirer à la fin du projet évidemment)
    _canals[0].exist = true;
    _canals[0].pass_only = false;
    _canals[0].invite_only = false;
    _canals[0].operators.push_back(string("pcapurro"));
    _canals[0].last_message = _canals[0].last_message + string(":revolver_ocelot PRIVMSG #canal1 :You're pretty good\r\n");
    _canals[0].last_message = _canals[0].last_message + string(":solid_snake PRIVMSG #canal1 :I know, right\r\n");
    _canals[0].topic = string("42");

    while (6)
    {
        int value = poll(_sockets_array, _clients_slots + 1, 100);
        if (value == -1)
        {
            if (end_ != true)
                printError(-1);
            else
                end_ = true;
            break ;
        }
        else if (value != 0)
        {
            for (int id = 0; value != 0; id++)
            {
                if (_sockets_array[id].revents != 0)
                {
                    if (id == 0)
                        addClient();
                    else
                        receiveData(id);
                    _sockets_array[id].revents = 0;
                    value--;
                }
            }
        }
        // verifyTimeOut();
        // sendPing();
    }
}

/*

EXPLICATION de la boucle

poll() -> prend en argument tous les sockets et bloque en mode 'surveillance' jusqu'à qu'un de ces sockets "sonne" (= qu'un client envoie une string)
Si déblocage de poll() :
1. value == -1 : erreur de poll(), arrêt du serveur

2. value == [nombre de sockets clients qui sonnent]
Dans ce cas là, on parcourt (l.75) tous les sockets des clients
    -> Si c'est la case zéro, c'est le socket du serveur qui sonne (= une première connexion entrante = un client à ajouter)
    = addClient() = récéption des infos du nouveau client et inscription dans le tableau de sockets
    -> Si c'est une autre case, c'est un socket déjà enregistré qui sonne (= un client qui envoie une commande à traiter)
    = receiveData() = réception de la string et exécution

*/