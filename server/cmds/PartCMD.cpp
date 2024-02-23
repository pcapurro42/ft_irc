/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:21:47 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/23 12:22:41 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePartCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 1)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 1)
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
    std::string channels = getArgument(cmd, 1);
    std::stringstream ss_channel(channels);
    int error = 0;
    while (std::getline(ss_channel, channels, ','))
    {
        error = 0;
        if (searchCanal(channels) == -1)
        {
            cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to part a channel (does not exist)." << endl;
            error = ERR_NOSUCHCHANNEL;
        }
        std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].members.begin(), _canals[searchCanal(channels)].members.end(), _clients_data[id].nickname);
        if (error == 0 && it != _canals[searchCanal(channels)].members.end()){
            _canals[searchCanal(channels)].members.erase(it);
            cout << getTime() << _clients_data[id].nickname << " has left " << channels << endl;
            sendToEveryone(_clients_data[id].nickname + " \x1D has left \x0f " + channels + ".\r\n", id, true);
            if ((it = std::find(_canals[searchCanal(channels)].operators.begin(), _canals[searchCanal(channels)].operators.end(), _clients_data[id].nickname)) != _canals[searchCanal(channels)].operators.end())
                _canals[searchCanal(channels)].operators.erase(it);
        }
        else if (error == 0)
        {
            cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to part a channel (not in channel)." << endl;
            error = ERR_NOTONCHANNEL;
        }
        if (error != 0)
            sendError(string("PART " + channels).c_str(), id, error);
    }
    return (0);
}