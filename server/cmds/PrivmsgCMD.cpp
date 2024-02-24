/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:46:37 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/24 21:30:37 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

std::string Server::getMessage(std::string cmd)
{
    std::string message = "";
    int i = 2;
    while(i < 202 && getArgument(cmd, i) != "") // max 200 mots
    {
        message = message + getArgument(cmd, i);
        if (getArgument(cmd, i + 1) != "")
            message = message + " ";
        i++;
    }
    return (message);
}

int Server::executePrivmsgCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb < 2)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }
    if (_clients_data[id].authentified != true)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified != true)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << endl;
        return (ERR_NOTIDENTIFIED);
    }
    std::string recipient = getArgument(cmd, 1);
    std::stringstream ss_recipient(recipient);
    while (std::getline(ss_recipient, recipient, ','))
    {
        if (recipient[0] == '#')
        {
            if (searchCanal(recipient) == -1)
            {
                cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (does not exist)." << endl;
                return (ERR_CANNOTSENDTOCHAN);
            }
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(recipient)].members.begin(), _canals[searchCanal(recipient)].members.end(), _clients_data[id].nickname);
            if (it == _canals[searchCanal(recipient)].members.end())
            {
                cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (not in channel)." << endl;
                return (ERR_NOSUCHNICK);
            }
            std::string message = getMessage(cmd);
            std::string msg = ":" + _clients_data[id].nickname + " PRIVMSG " + recipient + " :" + message + "\r\n";
            for (it = _canals[searchCanal(recipient)].members.begin(); it != _canals[searchCanal(recipient)].members.end(); it++)
                if (*it != _clients_data[id].nickname)
                    send(_sockets_array[searchClient(*it) + 1].fd, msg.c_str(), msg.size(), 0);
            cout << getTime() << _clients_data[id].nickname << " sent a message to " << recipient << " : " << message << endl;
        }
        else
        {
            if (searchClient(recipient) == -1)
            {
                cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (does not exist)." << endl;
                return (ERR_NOSUCHNICK);
            }
            std::string message = getMessage(cmd);
            std::string msg = ":" + _clients_data[id].nickname + " PRIVMSG " + recipient + " :" + message + "\r\n";
            send(_sockets_array[searchClient(recipient) + 1].fd, msg.c_str(), msg.size(), 0);
            cout << getTime() << _clients_data[id].nickname << " sent a message to " << recipient << " : " << message << endl;
        }
    }
    return (0);
}
