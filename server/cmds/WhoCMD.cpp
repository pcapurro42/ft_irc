/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCMD.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 02:16:29 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 02:29:51 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeWhoCommand(std::string cmd, int id)
{
    if (std::count(cmd.begin(), cmd.end(), ' ') > 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << std::endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (std::count(cmd.begin(), cmd.end(), ' ') < 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }
    else if (getArgument(cmd, 1)[0] != '#')
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid command." << std::endl;
        return (ERR_INVALIDCOMMAND);
    }

    std::string channel = getArgument(cmd, 1);
    std::vector<std::string> members = _canals[searchCanal(channel)].members;
    std::vector<std::string> ops = _canals[searchCanal(channel)].operators;
    std::string names;
    
    for (size_t i = 0; i < members.size(); ++i)
    {
        if (isOp(members[i], channel))
            names += "@" + members[i] + " ";
        else
            names += members[i] + " ";
    }

    std::string msg = ":server 353 " + _clients_data[id].nickname + " = " + channel + " :" + names + "\r\n";
    send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
    msg = ":server 366 " + _clients_data[id].nickname + " " + channel + " :End of /NAMES list\r\n";
    send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);

    return (0);
}