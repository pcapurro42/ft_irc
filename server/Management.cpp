/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Management.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:35:00 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 01:07:46 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::addSocket(int socket)
{
    int id = 0;
    for (; id != SLOTS_LIMIT + 1; id++)
    {
        if (_sockets_array[id].fd == -6)
        {
            _sockets_array[id].fd = socket;
            _sockets_array[id].events = POLLIN;
            _sockets_array[id].revents = 0;
            if (id != 0)
                cout << "Client #" << _clients_nb + 1 << " joined the server " << "(" << _clients_slots + 1 << "/" << SLOTS_LIMIT << ")." << endl;
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
    if (_clients_slots == SLOTS_LIMIT)
    {
        cout << "A client couldn't connect: server is full (" << _clients_slots << "/" << SLOTS_LIMIT << ")." << endl;
        send(client_socket, "471", 4, 0);
        close(client_socket);
    }
    else
    {
        int id = addSocket(client_socket);

        _clients_data[id - 1].connected = true;
        _clients_data[id - 1].number = _clients_nb + 1;

        _clients_data[id - 1].nickname = "Client #" + convertNumberToString(_clients_nb + 1);

        _clients_slots++;
        _clients_nb++;
    }
}

void    Server::removeClient(int id, int value)
{
    if (value == 0)
        cout << _clients_data[id - 1].nickname << " left the server (" << _clients_slots - 1 << "/" << SLOTS_LIMIT << ")." << endl;
    if (value == 1)
        cout << _clients_data[id - 1].nickname << " disconnected." << endl;

    if (_clients_data[id - 1].identified == true)
    {
        for (int i = 0; i != CANALS_LIMIT; i++)
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
        sendToEveryone(string (": " + _clients_data[id - 1].nickname + " \x1Dleft the server\x0f\r\n"), id - 1, true);
    }

    _clients_data[id - 1].nickname.clear();
    _clients_data[id - 1].username.clear();
    _clients_data[id - 1].realname.clear();

    _clients_data[id - 1].authentified = false;
    _clients_data[id - 1].identified = false;
    _clients_data[id - 1].connected = false;

    removeSocket(id);

    _clients_slots--;
}
