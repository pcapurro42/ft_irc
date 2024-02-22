/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:21:47 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:44:51 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePartCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');

    if (space_nb > 1)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 1)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }

    string canal = getArgument(cmd, 1);
   
    if (std::count(cmd.begin(), cmd.end(), '#') != 1 || canal.size() > 200)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    if (searchCanal(canal) == -1)
    {
        cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << endl;
        return (ERR_NOSUCHCHANNEL);
    }

    if (_clients_data[id].authentified == false)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified == false)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << endl;
        return (ERR_NOPRIVILEGES);
    }
    
    canal = getArgument(cmd, 1);
    int i = 0;
    while (i != MAX_CANALS)
    {
        if (_canals[i].exist == true && _canals[i].name == canal)
            break ;
        i++;
    }
    vector<string>::iterator j = std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id].nickname);
    if (j == _canals[i].members.end())
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to leave " << _canals[i].name << " (already left)." << endl;
        return (ERR_NOTONCHANNEL);
    }
    else
    {
        _canals[i].members.erase(j);
        cout << _clients_data[id].nickname << " left " << _canals[i].name << "." << endl;
        
        string message = ":" + _clients_data[id].nickname + " PART " + _canals[i].name + "\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);

        sendToEveryone(": " + _clients_data[id].nickname + " \x1Dleft " + _canals[i].name + ".\x0f\r\n", id, false);
    }
    return (0);
}