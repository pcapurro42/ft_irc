/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCMD.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:31 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 18:22:46 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::verifyInviteCMD(string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), '#') == 0)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');

    if (space_nb > 2)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 2)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }

    string nickname = getArgument(cmd, 1);
    string canalname = getArgument(cmd, 2);

    if (searchClient(nickname) == -1)
    {
        cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent user." << endl;
        return (ERR_NOSUCHNICK);
    }
    if (searchCanal(canalname) == -1)
    {
        cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << endl;
        return (ERR_NOSUCHCHANNEL);
    }
    return (0);
}

int Server::executeInviteCommand(string cmd, int id)
{
    int value = verifyInviteCMD(cmd, id);
    if (value != 0)
        return (value);
    else
    {
        if (_clients_data[id].identified != true || _clients_data[id].authentified != true)
        {
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
        }
        string sender = _clients_data[id].nickname;
        string target = getArgument(cmd, 1);
        string canal = getArgument(cmd, 2);

        int canal_id = searchCanal(canal);
        vector<string>::iterator i = std::find(_canals[canal_id].members.begin(), _canals[canal_id].members.end(), sender);
        if (i == _canals[canal_id].members.end())
        {
            cout << "Error! " << sender << " failed to invite someone to " << canal << " (not member of it)." << endl;
            return (ERR_NOTONCHANNEL);
        }
        else if (_canals[canal_id].invite_only == false)
        {
            cout << "Error! " << sender << " failed to invite someone to " << canal << " (channel is public or requires a password)." << endl;
            return (ERR_UNKNOWNCOMMAND);
        }
        else if (std::find(_canals[canal_id].members.begin(), _canals[canal_id].members.end(), target) != _canals[canal_id].members.end())
        {
            cout << "Error! " << sender << " failed to invite someone to " << canal << " (already member)." << endl;
            return (ERR_UNKNOWNCOMMAND);
        }
        else if (std::find(_canals[canal_id].invited.begin(), _canals[canal_id].invited.end(), target) != _canals[canal_id].invited.end())
        {
            cout << "Error! " << sender << " failed to invite someone to " << canal << " (already invited)." << endl;
            return (ERR_UNKNOWNCOMMAND);
        }
        _canals[canal_id].invited.push_back(target);
        cout << "Error! " << sender << " invited " << target << " to join " << canal << "." << endl;

        string message = ": " + sender + " " + target + " " + canal + "\r\n";
        send(_sockets_array[searchClient(target) + 1].fd, message.c_str(), message.size(), 0);
    }
    return (0);
}
