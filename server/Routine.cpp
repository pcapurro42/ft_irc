/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:10:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 23:56:19 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool end_;

void    Server::removeEmptyChannels(void)
{
    for (int i = 0; i != MAX_CANALS; i++)
    {
        if (_canals[i].members.size() == 0 && _canals[i].exist == true)
        {
            std::cout << getTime() << _canals[i].name << " has been removed." << std::endl;

            _canals[i].exist = false;

            _canals[i].name.clear();
            _canals[i].topic.clear();
            _canals[i].invited.clear();
            _canals[i].members.clear();
            _canals[i].operators.clear();
            _canals[i].topic.clear();
            _canals[i].password.clear();

            _canals[i].max = MAX_CLIENTS;
            _canals[i].invite_only = false;
            _canals[i].op_topic = false;
            _canals[i].pass_only = false;
        }
    }
}

void    Server::verifyTimeOut(void)
{
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].connected == true)
        {
            if (_clients_data[i].ping == false && _clients_data[i].ping_nb == 3)
            {
                std::cout << getTime() << _clients_data[i].nickname << " lost connection (time out) (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << std::endl;
                removeClient(i + 1);

                std::string msg = _clients_data[i].nickname + " \x1Dleft the server.\x0f\r\n";
                sendToEveryone(msg, i + 1, true);
            }
            else
                _clients_data[i].ping = false;
        }
    }
}

void    Server::sendPing(void)
{
    std::string message;
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].connected == true)
        {
            message = "PING :" + _clients_data[i].nickname + "\r\n";
            if (_clients_data[i].connected == true)
                send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
            _clients_data[i].ping_nb++;
            // std::cout << getTime() << "PING sent to " << _clients_data[i].nickname << std::endl;
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
            std::cerr << "Error! Couldn't receive data from Client #" << _clients_nb + 1 << "." << std::endl;
        else if (value == 0)
        {
            std::cout << getTime() << _clients_data[id - 1].nickname << " lost connection (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << std::endl;

            std::string msg = _clients_data[id - 1].nickname + " \x1Dleft the server.\x0f\r\n";
            sendToEveryone(msg, id, false);

            removeClient(id);
        }
        else
        {
            std::vector<std::string>  cmds = rectifyInput(std::string(buffer));
            for (std::vector<std::string>::iterator k = cmds.begin(); k != cmds.end(); k++)
            {
                std::string cmd = *k;
                if (std::count(cmd.begin(), cmd.end(), '\n') == 0)
                {
                    std::cout << getTime() << "Partial command received from " << _clients_data[id - 1].nickname << ". Conserving it." << std::endl;
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
                    std::string cmd_name = getArgument(cmd, 0);
                    executeCommand(cmd, cmd_name, id);
                }
            }
        }
    }
    else if (_sockets_array[id].revents == POLLERR || _sockets_array[id].revents == POLLNVAL)
    {
        std::cout << getTime() << _clients_data[id - 1].nickname << " lost connection (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << std::endl;

        std::string msg = _clients_data[id - 1].nickname + " \x1Dleft the server.\x0f\r\n";
        sendToEveryone(msg, id, false);

        removeClient(id);
    }
}

void    Server::LoopRoutine(void)
{
    addSocket(_server_socket);
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
        else
        {
            removeEmptyChannels();
            verifyTimeOut();
            sendPing();
        }
    }
    sendToEveryone("Server is shutting down...\r\n", 0, true);
}
