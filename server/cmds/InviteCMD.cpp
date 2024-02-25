/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCMD.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:31 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:45 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::verifyInviteCMD(std::string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), '#') == 0)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << std::endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');

    if (space_nb > 2)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << std::endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 2)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }

    std::string nickname = getArgument(cmd, 1);
    std::string canalname = getArgument(cmd, 2);

    if (searchClient(nickname) == -1)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent user." << std::endl;
        return (ERR_NOSUCHNICK);
    }
    if (searchCanal(canalname) == -1)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << std::endl;
        return (ERR_NOSUCHCHANNEL);
    }
    return (0);
}

int Server::executeInviteCommand(std::string cmd, int id)
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
                std::cout << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << std::endl;
                return (ERR_NOTREGISTERED);
            }
            if (_clients_data[id].identified == false)
            {
                std::cout << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << std::endl;
                return (ERR_NOPRIVILEGES);
            }
        }
        std::string sender = _clients_data[id].nickname;
        std::string target = getArgument(cmd, 1);
        std::string canal = getArgument(cmd, 2);

        int canal_id = searchCanal(canal);
        std::vector<std::string>::iterator i = std::find(_canals[canal_id].members.begin(), _canals[canal_id].members.end(), sender);
        if (i == _canals[canal_id].members.end())
        {
            std::cout << "Error! " << sender << " failed to invite someone to " << canal << " (not member of it)." << std::endl;
            return (ERR_NOTONCHANNEL);
        }
        else if (_canals[canal_id].invite_only == false)
        {
            std::cout << "Error! " << sender << " failed to invite someone to " << canal << " (channel is public or requires a password)." << std::endl;
            return (ERR_UNKNOWNCOMMAND);
        }
        else if (std::find(_canals[canal_id].members.begin(), _canals[canal_id].members.end(), target) != _canals[canal_id].members.end())
        {
            std::cout << "Error! " << sender << " failed to invite someone to " << canal << " (already member)." << std::endl;
            return (ERR_UNKNOWNCOMMAND);
        }
        else if (std::find(_canals[canal_id].invited.begin(), _canals[canal_id].invited.end(), target) != _canals[canal_id].invited.end())
        {
            std::cout << "Error! " << sender << " failed to invite someone to " << canal << " (already invited)." << std::endl;
            return (ERR_UNKNOWNCOMMAND);
        }
        _canals[canal_id].invited.push_back(target);
        std::cout << "Error! " << sender << " invited " << target << " to join " << canal << "." << std::endl;

        std::string message = ": " + sender + " " + target + " " + canal + "\r\n";
        send(_sockets_array[searchClient(target) + 1].fd, message.c_str(), message.size(), 0);
    }
    return (0);
}
