/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:46:37 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 15:42:57 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

std::string Server::getMessage(std::string cmd) const
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
    message = message.substr(1);
    return (message);
}

int Server::executePrivmsgCommand(std::string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb < 2)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }
    std::string recipient = getArgument(cmd, 1);
    std::stringstream ss_recipient(recipient);
    int error = 0;
    while (std::getline(ss_recipient, recipient, ','))
    {
        if (recipient[0] == '#')
        {
            if (searchCanal(recipient) == -1)
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (does not exist)." << std::endl;
                error = ERR_CANNOTSENDTOCHAN;
            }
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(recipient)].members.begin(), _canals[searchCanal(recipient)].members.end(), _clients_data[id].nickname);
            if (error == 0 && it == _canals[searchCanal(recipient)].members.end())
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (not in channel)." << std::endl;
                error = ERR_NOSUCHCHANNEL;
            }
            if (error == 0){
                std::string message = getMessage(cmd);
                std::string msg = ":" + _clients_data[id].nickname + " PRIVMSG " + recipient + " :" + message + "\r\n";
                for (it = _canals[searchCanal(recipient)].members.begin(); it != _canals[searchCanal(recipient)].members.end(); it++)
                    if (*it != _clients_data[id].nickname)
                        send(_sockets_array[searchClient(*it) + 1].fd, msg.c_str(), msg.size(), 0);
                std::cout << getTime() << _clients_data[id].nickname << " sent a message to " << recipient << " : " << message << std::endl;
            }
            if (error != 0)
                sendError(std::string("PRIVMSG " + recipient).c_str(), id + 1, error);
        }
        else
        {
            if (searchClient(recipient) == -1)
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (does not exist)." << std::endl;
                sendError(std::string("PRIVMSG " + recipient).c_str(), id + 1, ERR_NOSUCHNICK);
            }
            else{
                std::string message = getMessage(cmd);
                if (BOT == 1 && recipient == "Gipiti_bot"){
                    botTOTD(id, message);
                    return (0);
                }
                std::cout << getTime() <<  std::count(message.begin(), message.end(), ' ') << std::endl;
                if (message.find("DCC SEND") == 0 && std::count(message.begin(), message.end(), ' ') == 5)
                    message = "\x01" + message + "\x01";
                std::string msg = ":" + _clients_data[id].nickname + " PRIVMSG " + recipient + " :" + message + "\r\n";
                send(_sockets_array[searchClient(recipient) + 1].fd, msg.c_str(), msg.size(), 0);
                std::cout << getTime() << _clients_data[id].nickname << " sent a message to " << recipient << " : " << message << std::endl;
            }
        }
    }
    return (0);
}
