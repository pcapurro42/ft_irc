/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCMD.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:43 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 04:04:22 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeTopicCommand(std::string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb < 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }
    std::string channels = getArgument(cmd, 1);
    if (searchCanal(channels) == -1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (channel not found)." << std::endl;
        return (ERR_NOSUCHCHANNEL);
    }
    else if (searchCanal(channels) != -1){
        if (getArgument(cmd, 2) == "")
        {
            std::cout << getTime() << _clients_data[id].nickname << " requested the topic of " << channels << "." << std::endl;
            std::string msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + " :" + _canals[searchCanal(channels)].topic + "\r\n";
            if (_canals[searchCanal(channels)].topic == "")
                msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + " :No topic is set\r\n";
            send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
        }
        else
        {
            std::string topic = getMessage(cmd);
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].members.begin(), _canals[searchCanal(channels)].members.end(), _clients_data[id].nickname);
            if (it != _canals[searchCanal(channels)].members.end())
            {
                if (_canals[searchCanal(channels)].op_topic == true)
                {
                    std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].operators.begin(), _canals[searchCanal(channels)].operators.end(), _clients_data[id].nickname);
                    if (it != _canals[searchCanal(channels)].operators.end())
                    {
                        _canals[searchCanal(channels)].topic = topic;
                        std::cout << getTime() << _clients_data[id].nickname << " changed the topic of " << channels << " to " << topic << "." << std::endl;
                        std::string msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + "set new topic :" + topic + "\r\n";
                        sendToEveryone(msg, id, 1);
                    }
                    else
                    {
                        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not operator)." << std::endl;
                        return (ERR_CHANOPRIVSNEEDED);
                    }
                }
                else{
                    _canals[searchCanal(channels)].topic = topic;
                    std::cout << getTime() << _clients_data[id].nickname << " changed the topic of " << channels << " to " << topic << "." << std::endl;
                    std::string msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + "set new topic :" + topic + "\r\n";
                    sendToEveryone(msg, id, 1);
                }
            }
            else
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not in the channel)." << std::endl;
                return (ERR_NOTONCHANNEL);
            }
        }
    }
    return (0);
}
