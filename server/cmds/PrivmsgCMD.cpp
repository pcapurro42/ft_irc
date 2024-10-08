/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:46:37 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 21:39:07 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

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
                error = ERR_NOSUCHCHANNEL;
            }
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(recipient)].members.begin(), _canals[searchCanal(recipient)].members.end(), _clients_data[id].nickname);
            if (error == 0 && it == _canals[searchCanal(recipient)].members.end())
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (not in channel)." << std::endl;
                error = ERR_NOSUCHCHANNEL;
            }
            if (error == 0 && getFullArgument(cmd, 2).size() > 200)
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (invalid length)." << std::endl;
                error = ERR_INVALIDCOMMAND;
            }
            if (error == 0){
                std::string message = getFullArgument(cmd, 2);
                std::string msg;

                if (message[0] == ':')
                {
                    message = message.c_str() + 1;
                    msg = ":" + _clients_data[id].nickname + " PRIVMSG " + recipient + " :" + message + "\r\n";
                    sendToEveryChannelMembers(msg, recipient, _clients_data[id].nickname, false);
                }
                else
                {
                    msg = ":" + _clients_data[id].nickname + " PRIVMSG " + recipient + " :" + message + "\r\n";
                    sendToEveryChannelMembers(msg, recipient, _clients_data[id].nickname, true);
                }
                    
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
            else if (getFullArgument(cmd, 2).size() > 200)
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to send a message (invalid length)." << std::endl;
                sendError(std::string("PRIVMSG " + recipient + " " + getFullArgument(cmd, 2)).c_str(), id + 1, ERR_INVALIDCOMMAND);
            }
            else
            {
                std::string message = getFullArgument(cmd, 2);
                if (message[0] == ':')
                    message = message.c_str() + 1;
                
                if (BOT == 1 && recipient == "Gipiti_bot")
                {
                    botTOTD(id, message);
                    continue ;
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
