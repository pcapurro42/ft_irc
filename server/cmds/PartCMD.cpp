/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:21:47 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 23:54:01 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePartCommand(std::string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 2)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << std::endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }
    if (_clients_data[id].authentified != true)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << std::endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified != true)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << std::endl;
        return (ERR_NOTIDENTIFIED);
    }
    std::string channels = getArgument(cmd, 1);
    std::string reason = getArgument(cmd, 2);
    std::stringstream ss_channel(channels);
    int error = 0;
    while (std::getline(ss_channel, channels, ','))
    {
        error = 0;
        if (searchCanal(channels) == -1)
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to part a channel (does not exist)." << std::endl;
            error = ERR_NOSUCHCHANNEL;
        }
        std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].members.begin(), _canals[searchCanal(channels)].members.end(), _clients_data[id].nickname);
        if (error == 0 && it != _canals[searchCanal(channels)].members.end()){
            _canals[searchCanal(channels)].members.erase(it);
            if (space_nb == 2){
                std::cout << getTime() << _clients_data[id].nickname << " has left " << channels << " :" << reason << std::endl;
                std::string msg = ":" + _clients_data[id].nickname + " PART " + channels + " :" + reason + "\r\n";
                send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
            }
            else{
                std::cout << getTime() << _clients_data[id].nickname << " has left " << channels << " :leaving" << std::endl;
                std::string msg = ":" + _clients_data[id].nickname + " PART " + channels + "\r\n";
                send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
            }
            
            std::string msg = ":" + _clients_data[id].nickname + " PART " + channels + "\r\n";
            std::vector<std::string>::iterator k = _canals[searchCanal(channels)].members.begin();
            while (k != _canals[searchCanal(channels)].members.end())
                send(_sockets_array[searchClient(*k) + 1].fd, msg.c_str(), msg.size(), 0), k++;
            sendToEveryone(_clients_data[id].nickname + " \x1Dhas left \x0f" + channels + ".\r\n", id, true);
            
            if ((it = std::find(_canals[searchCanal(channels)].operators.begin(), _canals[searchCanal(channels)].operators.end(), _clients_data[id].nickname)) != _canals[searchCanal(channels)].operators.end())
                _canals[searchCanal(channels)].operators.erase(it);
        }
        else
        {
            if (error == 0){
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to part a channel (not in channel)." << std::endl;
                error = ERR_NOTONCHANNEL;
            }
        }
        if (error != 0)
            sendError(std::string("PART " + channels).c_str(), id + 1, error);
    }
    return (0);
}