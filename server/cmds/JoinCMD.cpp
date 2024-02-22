/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:17:27 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 16:53:53 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeJoinCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 2)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 1)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }
    if (_clients_data[id].authentified != true)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified != true)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << endl;
        return (ERR_NOTIDENTIFIED);
    }

    std::string channels = getArgument(cmd, 1);
    std::string passwords = getArgument(cmd, 2);
    std::stringstream ss_channel(channels);
    std::stringstream ss_password(passwords);
    int error = 0;
    while (std::getline(ss_channel, channels, ','))
    {
        error = 0;
        std::getline(ss_password, passwords, ',');
        if (searchCanal(channels) == -1)
        {
            cout << "Error! " << _clients_data[id].nickname << " failed to join a channel (does not exist)." << endl;
            error = ERR_NOSUCHCHANNEL;
        }
        if (_canals[searchCanal(channels)].members.size() >= _canals[searchCanal(channels)].max) 
        {
            cout << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (channel is full)." << endl;
            error = ERR_CHANNELISFULL;
        }
        std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].invited.begin(), _canals[searchCanal(channels)].invited.end(), _clients_data[id].nickname);
        if (_canals[searchCanal(channels)].invite_only == true && it == _canals[searchCanal(channels)].invited.end())
        {
            cout << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (invite only)." << endl;
            error = ERR_INVITEONLYCHAN;
        }
        it = std::find(_canals[searchCanal(channels)].members.begin(), _canals[searchCanal(channels)].members.end(), _clients_data[id].nickname);
        if (it != _canals[searchCanal(channels)].members.end())
        {
            cout << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (already in channel)." << endl;
            error = ERR_ALREADYINCHANNEL;
        }
        if (_canals[searchCanal(channels)].pass_only == true && _canals[searchCanal(channels)].password != passwords)
        {
            cout << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (incorrect password)." << endl;
            error = ERR_BADCHANNELKEY;
        }
        
        if (error == 0)
        {
            _canals[searchCanal(channels)].members.push_back(_clients_data[id].nickname);
            cout << _clients_data[id].nickname << " joined " << channels << "." << endl;
        
            std::string msg = ":" + _clients_data[id].nickname + " JOIN " + channels;
            send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
            sendToEveryone(_clients_data[id].nickname + " \x1Djoined " + channels + ".\x0f\r\n", id, true);

            // TOPIC + MEMBERS
        }
        else
            sendError(string("JOIN " + channels).c_str(), id, error);
    }
    return (0);
}
