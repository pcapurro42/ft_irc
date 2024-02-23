/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:50:01 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 18:39:23 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool end_;

Server::Server(const int port, const char *password)
{
    if (validateInput(port, password) != true)
        _fail = true;
    else
    {
        printMessage("Launching server", false);
        _fail = false;

        _port = port;
        _password = string(password);

        _clients_nb = 0;
        _clients_slots = 0;

        for (int i = 0; i != MAX_CLIENTS + 1; i++)  // -6 chiffre aléatoire pour savoir si la case est allouée par un client ou pas
            _sockets_array[i].fd = -6;
        for (int i = 0; i != MAX_CANALS; i++) // mise à zéro de tous les channels
        {
            _canals[i].exist = false;
            _canals[i].max = MAX_CLIENTS;
        }

        initializeServer();
    }
}

Server::Server(const Server &original)
{
    *this = original;
}

Server  &Server::operator=(const Server &original)
{
    printMessage("Cloning server", true);
    return (*this);
}

Server::~Server()
{
    printMessage("Destroying server", true);
    
    close(_server_socket);
    for (int i = 1; i != MAX_CLIENTS + 1; i++)
    {
        if (_sockets_array[i].fd != -6)
            close(_sockets_array[i].fd);
    }
}

void    Server::setFail(void)
{
    _fail = true;
}

bool    Server::end(void) const
{
    if (end_ == false)
        return (false);
    return (true);
}

bool    Server::fail(void) const
{
    if (_fail == false)
        return (false);
    return (true);
}
