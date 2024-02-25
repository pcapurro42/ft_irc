/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:10:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 17:34:14 by pcapurro         ###   ########.fr       */
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
            if (_clients_data[i].ping == false && _clients_data[i].ping_nb == 3)
            {
                cout << getTime() << _clients_data[i].nickname << " lost connection (time out) (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;
                removeClient(i + 1);

                string msg = _clients_data[i].nickname + " \x1Dleft the server.\x0f\r\n";
                sendToEveryone(msg, i + 1, true);
            }
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
        if (_clients_data[i].connected == true && _clients_data[i].ping == false)
        {
            message = "PING :" + _clients_data[i].nickname + "\r\n";
            if (_clients_data[i].connected == true)
                send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
            _clients_data[i].ping_nb++;
            // cout << getTime() << "PING sent to " << _clients_data[i].nickname << endl;
        }
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
        {
            cout << getTime() << _clients_data[id - 1].nickname << " lost connection (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;

            string msg = _clients_data[id - 1].nickname + " \x1Dleft the server.\x0f\r\n";
            sendToEveryone(msg, id, false);

            removeClient(id);
        }
        else
        {
            vector<string>  cmds = rectifyInput(string(buffer));
            for (vector<string>::iterator k = cmds.begin(); k != cmds.end(); k++)
            {
                string cmd = *k;
                if (std::count(cmd.begin(), cmd.end(), '\n') == 0)
                {
                    cout << getTime() << "Partial command received from " << _clients_data[id - 1].nickname << ". Conserving it." << endl;
                    _clients_data[id - 1].last_command = _clients_data[id - 1].last_command + cmd;
                }
                else
                {
                    if (cmds.size() == 1 && _clients_data[id - 1].last_command.empty() == false)
                    {
                        cmd = _clients_data[id - 1].last_command + cmd;
                        _clients_data[id - 1].last_command.clear();
                    }
                    while (std::count(cmd.begin(), cmd.end(), '\n') != 0)
                        cmd.erase(std::find(cmd.begin(), cmd.end(), '\n'));
                    string cmd_name = getArgument(cmd, 0);
                    executeCommand(cmd, cmd_name, id);
                }
            }
        }
    }
    else if (_sockets_array[id].revents == POLLERR || _sockets_array[id].revents == POLLNVAL)
    {
        cout << getTime() << _clients_data[id - 1].nickname << " lost connection (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;

        string msg = _clients_data[id - 1].nickname + " \x1Dleft the server.\x0f\r\n";
        sendToEveryone(msg, id, false);

        removeClient(id);
    }
}

void    Server::LoopRoutine(void)
{
    addSocket(_server_socket);

    _canals[0].name = "#canal1";
    _canals[0].exist = true;
    _canals[0].pass_only = false;
    _canals[0].invite_only = false;
    _canals[0].operators.push_back(string("pcapurro"));
    _canals[0].last_message = _canals[0].last_message + string(":revolver_ocelot PRIVMSG #canal1 :You're pretty good\r\n");
    _canals[0].last_message = _canals[0].last_message + string(":solid_snake PRIVMSG #canal1 :I know, right\r\n");
    _canals[0].topic = string("42");

    _canals[1].name = "#canal2";
    _canals[1].exist = true;
    _canals[1].pass_only = false;
    _canals[1].invite_only = false;
    _canals[1].topic = string("21");

    while (6)
    {
        int value = poll(_sockets_array, _clients_slots + 1, 3000);
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
