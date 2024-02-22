/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:20:08 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 13:25:30 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeNickCommand(string cmd, int id)
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

    string clientname = getArgument(cmd, 1);
    if (clientname.size() > 42)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to set a nickname (invalid length)." << endl;
        return (ERR_ERRONEUSNICKNAME);
    }
    if (searchClient(clientname) != -1)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to set a nickname (already taken)." << endl;
        return (ERR_NICKCOLLISION);
    }

    if (_clients_data[id].authentified != true)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    else
    {
        string nickname = getArgument(cmd, 1);
        string oldnickname = _clients_data[id].nickname;
        _clients_data[id].nickname = nickname;

        cout << _clients_data[id].nickname << " set his nickname to '" << nickname << "'." << endl;

        if (_clients_data[id].username.empty() != true && _clients_data[id].identified != true)
        {
            _clients_data[id].identified = true;

            sendToEveryone(": " + _clients_data[id].nickname + " \x1Djoined the server.\x0f\r\n", id, true);
            
            string message = "001 " + _clients_data[id].nickname + " :\r\n";
            send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
            
            return (0);
        }

        if (oldnickname.empty() != true)
        {
            for (int i = 0; i != MAX_CANALS; i++)
            {
                vector<string>::iterator j;

                j = std::find(_canals[i].members.begin(), _canals[i].members.end(), oldnickname);
                if (j != _canals[i].members.end())
                    _canals[i].members.erase(j), _canals[i].members.push_back(nickname);
                
                j = std::find(_canals[i].operators.begin(), _canals[i].operators.end(), oldnickname);
                if (j != _canals[i].operators.end())
                    _canals[i].operators.erase(j), _canals[i].operators.push_back(nickname);
                
                j = std::find(_canals[i].invited.begin(), _canals[i].invited.end(), oldnickname);
                if (j != _canals[i].invited.end())
                    _canals[i].invited.erase(j), _canals[i].invited.push_back(nickname);
            }
        }
    }
    return (0);
}