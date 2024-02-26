/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Management.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:35:00 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 16:56:45 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::addSocket(int socket)
{
    int id = 0;
    for (; id != MAX_CLIENTS + 1; id++)
    {
        if (_sockets_array[id].fd == -6)
        {
            _sockets_array[id].fd = socket;
            _sockets_array[id].events = POLLIN;
            _sockets_array[id].revents = 0;
            if (id != 0)
                std::cout << getTime() << "Client #" << _clients_nb + 1 << " joined the server " << "(" << _clients_slots + 1 << "/" << MAX_CLIENTS << ")." << std::endl;
            break ;
        }
    }
    return (id);
}

void    Server::removeSocket(int id)
{
    close(_sockets_array[id].fd);
    _sockets_array[id].fd = -6;
}

void    Server::addClient(void)
{
    int client_socket;

    client_socket = accept(_server_socket, NULL, NULL);
    fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL, 0) | O_NONBLOCK);
    if (_clients_slots == MAX_CLIENTS)
    {
        std::cout << getTime() << "A client couldn't connect: server is full (" << _clients_slots << "/" << MAX_CLIENTS << ")." << std::endl;
        std::string message = ": 421 ERROR Server is full.\r\n";
        send(client_socket, message.c_str(), message.size(), 0);
        close(client_socket);
    }
    else
    {
        int id = addSocket(client_socket);

        _clients_data[id - 1].connected = true;
        _clients_data[id - 1].ping = true;
        _clients_data[id - 1].authentified = false;
        _clients_data[id - 1].identified = false;
        _clients_data[id - 1].set_nickname = false;
        _clients_data[id - 1].set_username = false;
        _clients_data[id - 1].number = _clients_nb + 1;
        _clients_data[id - 1].nickname = "Client #" + convertNumberToString(_clients_nb + 1);

        _clients_slots++;
        _clients_nb++;
    }
}

void    Server::removeClient(int id)
{
    if (_clients_data[id - 1].identified == true)
    {
        for (int i = 0; i != MAX_CANALS; i++)
        {
            std::vector<std::string>::iterator j;
            
            j = std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id - 1].nickname);
            if (j != _canals[i].members.end())
            {
                _canals[i].members.erase(j);
                std::string msg = ":" + _clients_data[id - 1].nickname + " PART " + _canals[i].name + "\r\n";
                std::vector<std::string>::iterator k = _canals[i].members.begin();
                while (k != _canals[i].members.end())
                    send(_sockets_array[searchClient(*k) + 1].fd, msg.c_str(), msg.size(), 0), k++;
            }

            j = std::find(_canals[i].operators.begin(), _canals[i].operators.end(), _clients_data[id - 1].nickname);
            if (j != _canals[i].operators.end())
                _canals[i].operators.erase(j);

            j = std::find(_canals[i].invited.begin(), _canals[i].invited.end(), _clients_data[id - 1].nickname);
            if (j != _canals[i].invited.end())
                _canals[i].invited.erase(j);
        }
    }

    _clients_data[id - 1].connected = false;

    _clients_data[id - 1].nickname.clear();
    _clients_data[id - 1].username.clear();
    _clients_data[id - 1].realname.clear();

    _clients_data[id - 1].last_command.clear();

    removeSocket(id);

    _clients_slots--;
}
