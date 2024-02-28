/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:50:01 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 20:33:33 by pcapurro         ###   ########.fr       */
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
        _password = std::string(password);

        _clients_nb = 0;
        _clients_slots = 0;

        for (int i = 0; i != MAX_CLIENTS + 1; i++)
            _sockets_array[i].fd = -6;
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {            
            _clients_data[i].number = 0;
            _clients_data[i].authentified = false;
            _clients_data[i].identified = false;
            _clients_data[i].connected = false;
            _clients_data[i].ping = false;
            _clients_data[i].set_nickname = false;
            _clients_data[i].set_username = false;
        }
        for (int i = 0; i != MAX_CANALS; i++)
        {
            _canals[i].exist = false;
            _canals[i].max = MAX_CLIENTS;
            _canals[i].op_topic = false;
            _canals[i].invite_only = false;
            _canals[i].pass_only = false;
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
    
    this->_fail = original._fail;
    this->_password = original._password;
    this->_port = original._port;

    if (this->_fail == true)
        return (*this);
    else
    {
        this->_server_socket = original._server_socket;
        this->_sockets_array[0].fd = original._server_socket;
        this->_sockets_array[0].revents = 0;
        this->_sockets_array[0].fd = POLLIN;
    }
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
