/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:10:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 17:46:53 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool end_;

void    Server::verifyPing(void)
{
    ;
}

void    Server::receiveData(int id)
{
    if (_sockets_array[id].revents == POLLIN)
    {
        char    buffer[512];
        buffer[511] = '\0';
        int value = recv(_sockets_array[id].fd, buffer, 511, 0);
        if (value == -1)
            cerr << "Error! Couldn't receive data from Client #" << _clients_nb + 1 << "." << endl;
        else if (value == 0)
            removeClient(id, 0);
        else
        {
            int value = 0;
            while (buffer[value] != '\r' && buffer[value] != '\n' && buffer[value] != '\0')
                value++;
            buffer[value] = '\0';
            
            string cmd_name;
            for (int i = 0; buffer[i] != '\0' && buffer[i] != ' '; i++)
                cmd_name = cmd_name + buffer[i];
            
            executeCommand(buffer, cmd_name, id);
        }
    }
    else if (_sockets_array[id].revents == POLLERR || _sockets_array[id].revents == POLLNVAL)
        removeClient(id, 1);
}


void    Server::startLoopRoutine(void)
{
    addSocket(_server_socket);

    _canals[0].name = "#canal1";
    _canals[0].exist = true;
    _canals[0].pass_only = false;
    _canals[0].invite_only = false;
    _canals[0].operators.push_back(string("shalashaska"));

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
        verifyPing();
    }
}
