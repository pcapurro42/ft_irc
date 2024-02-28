/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:15 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 21:24:59 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::verifyKickCMD(std::string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), '#') == 0)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid command." << std::endl;
        return (ERR_INVALIDCOMMAND);
    }
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    
    if (space_nb < 3)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }

    std::string canal = getArgument(cmd, 1);
    std::string user = getArgument(cmd, 2);
    std::string reason = getFullArgument(cmd, 4);

    if (searchCanal(canal) == -1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << std::endl;
        return (ERR_NOSUCHCHANNEL);
    }
    if (searchClient(user) == -1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " searched for a non-existent user." << std::endl;
        return (ERR_NOSUCHNICK);
    }
    if (reason.size() > 200)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid command." << std::endl;
        return (ERR_INVALIDCOMMAND);
    }
    return (0);
}

int Server::executeKickCommand(std::string cmd, int id)
{
    int value = verifyKickCMD(cmd, id);
    if (value != 0)
        return (value);
    else
    {
        std::string canal = getArgument(cmd, 1);
        std::string member = getArgument(cmd, 2);
        std::string reason = getFullArgument(cmd, 4);

        if (reason.empty() == true)
            reason = "no reason";
        else if (reason[0] == ':')
            reason = reason.c_str() + 1;

        int i = 0;
        while (i != MAX_CANALS)
        {
            if (_canals[i].name == canal)
                break ;
            i++;
        }
        if (std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id].nickname) == _canals[i].members.end())
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to kick someone from " << canal << " (not member)." << std::endl;
            return (ERR_NOTONCHANNEL);
        }
        else if (std::find(_canals[i].members.begin(), _canals[i].members.end(), member) == _canals[i].members.end())
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to kick someone from " << canal << " (target not member)." << std::endl;
            return (ERR_NOTONCHANNEL);
        }
        else if (std::find(_canals[i].operators.begin(), _canals[i].operators.end(), _clients_data[id].nickname) != _canals[i].operators.end())
        {
            _canals[i].members.erase(std::find(_canals[i].members.begin(), _canals[i].members.end(), member));
            if (std::find(_canals[i].operators.begin(), _canals[i].operators.end(), member) != _canals[i].operators.end())
                _canals[i].operators.erase(std::find(_canals[i].operators.begin(), _canals[i].operators.end(), member));
            if (reason != "no reason")
                std::cout << getTime() << _clients_data[id].nickname << " kicked " << member << " from " << _canals[i].name <<  " (reason: '" << reason << "')." << std::endl;
            else
                std::cout << getTime() << _clients_data[id].nickname << " kicked " << member << " from " << _canals[i].name << " for no reason." << std::endl;

            int target_id = searchClient(member) + 1;
            std::string message = ":" + _clients_data[id].nickname + " KICK " + canal + " " + member + " :" + reason + "\r\n";
            send(_sockets_array[target_id].fd, message.c_str(), message.size(), 0);

            sendToEveryChannelMembers(message, _canals[i].name, _clients_data[id].nickname, true);
            sendToEveryone(_clients_data[id].nickname + " \x1Dhas left\x0f " + _canals[i].name + ".\r\n", id, true);
        }
        else
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to kick " << member << "(not operator)." << std::endl;
            return (ERR_NOPRIVILEGES);
        }
    }
    return (0);
}