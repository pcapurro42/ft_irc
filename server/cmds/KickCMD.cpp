/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:15 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 17:03:28 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::verifyKickCMD(string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), '#') == 0)
        return (ERR_UNKNOWNCOMMAND);
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    
    if (space_nb > 3)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 3)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }

    string canal = getArgument(cmd, 1);
    string user = getArgument(cmd, 2);
    string reason = getArgument(cmd, 3);

    if (searchCanal(canal) == -1)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " searched for a non-existent channel." << endl;
        return (ERR_NOSUCHCHANNEL);
    }
    if (searchClient(user) == -1)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " searched for a non-existent user." << endl;
        return (ERR_NOSUCHNICK);
    }
    if (reason.size() > 400)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " typed an invalid or unsupported command." << endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    return (0);
}

int Server::executeKickCommand(string cmd, int id)
{
    int value = verifyKickCMD(cmd, id);
    if (value != 0)
        return (value);
    else
    {
        if (_clients_data[id - 1].identified != true || _clients_data[id - 1].authentified != true)
        {
            if (_clients_data[id - 1].authentified == false)
            {
                cout << "Error! " << _clients_data[id - 1].nickname << " failed to request (not authentified)." << endl;
                return (ERR_NOTREGISTERED);
            }
            if (_clients_data[id - 1].identified == false)
            {
                cout << "Error! " << _clients_data[id - 1].nickname << " failed to request (not identified)." << endl;
                return (ERR_NOPRIVILEGES);
            }
        }
        string canal = getArgument(cmd, 1);
        string member = getArgument(cmd, 2);
        string reason = getArgument(cmd, 3);

        if (reason[0] == ':')
            reason = reason.c_str() + 1;

        int i = 0;
        while (i != MAX_CANALS)
        {
            if (_canals[i].name == canal)
                break ;
            i++;
        }
        if (std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id - 1].nickname) == _canals[i].members.end())
        {
            cout << "Error! " << _clients_data[id - 1].nickname << " failed to kick someone from " << canal << " (not member)." << endl;
            return (ERR_NOTONCHANNEL);
        }
        else if (std::find(_canals[i].operators.begin(), _canals[i].operators.end(), _clients_data[id - 1].nickname) != _canals[i].operators.end())
        {
            _canals[i].members.erase(std::find(_canals[i].members.begin(), _canals[i].members.end(), member));
            cout << _clients_data[id - 1].nickname << " kicked " << member << " from " << _canals[i].name <<  " (reason :'" << reason << "')." << endl;

            int target_id = searchClient(member) + 1;
            string message = ": " + _clients_data[id - 1].nickname + " KICK " + _canals[i].name + " " + member + "\r\n";
            send(_sockets_array[target_id].fd, message.c_str(), message.size(), 0);

            sendToEveryone(": " + _clients_data[id - 1].nickname + " \x1Dleft " + _canals[i].name + ".\x0f\r\n", id - 1, true);
        }
        else
        {
            cout << "Error! " << _clients_data[id - 1].nickname << " failed to kick " << member << "(not operator)." << endl;
            return (ERR_NOPRIVILEGES);
        }
    }
    return (0);
}