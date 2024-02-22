/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Management.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:35:00 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 19:02:33 by pcapurro         ###   ########.fr       */
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
                cout << getTime() << "Client #" << _clients_nb + 1 << " joined the server " << "(" << _clients_slots + 1 << "/" << MAX_CLIENTS << ")." << endl;
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
    if (_clients_slots == MAX_CLIENTS)
    {
        cout << getTime() << "A client couldn't connect: server is full (" << _clients_slots << "/" << MAX_CLIENTS << ")." << endl;
        send(client_socket, "Request denied: server is full\r\n", 4, 0);
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

void    Server::removeClient(int id, int value)
{
    if (value == 0)
        cout << getTime() << _clients_data[id - 1].nickname << " left the server (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;
    else if (value == 1)
        cout << getTime() << _clients_data[id - 1].nickname << " lost connection (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;
    else if (value == 2)
        cout << getTime() << _clients_data[id - 1].nickname << " lost connection (time out) (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;

    if (_clients_data[id - 1].identified == true)
    {
        for (int i = 0; i != MAX_CANALS; i++)
        {
            vector<string>::iterator j;
            
            j = std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id - 1].nickname);
            if (j != _canals[i].members.end())
                _canals[i].members.erase(j);

            j = std::find(_canals[i].operators.begin(), _canals[i].operators.end(), _clients_data[id - 1].nickname);
            if (j != _canals[i].operators.end())
                _canals[i].operators.erase(j);

            j = std::find(_canals[i].invited.begin(), _canals[i].invited.end(), _clients_data[id - 1].nickname);
            if (j != _canals[i].invited.end())
                _canals[i].invited.erase(j);
        }
    }

    _clients_data[id - 1].nickname.clear();
    _clients_data[id - 1].username.clear();
    _clients_data[id - 1].realname.clear();

    _clients_data[id - 1].authentified = false;
    _clients_data[id - 1].identified = false;
    _clients_data[id - 1].connected = false;
    _clients_data[id - 1].set_nickname = false;
    _clients_data[id - 1].set_username = false;

    removeSocket(id);

    _clients_slots--;
}
