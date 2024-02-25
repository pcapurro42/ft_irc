/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:20:32 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:21 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeQuitCommand(std::string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << std::endl;
        return (ERR_TOOMANYPARAMS);
    }
    else
    {
        if (std::count(cmd.begin(), cmd.end(), ' ') != 0)
        {
            std::string message = getArgument(cmd, 1);
            if (message[0] == ':')
                message = message.c_str() + 1;

            std::cout << getTime() << _clients_data[id].nickname << " left the server saying '" << message << "' (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << std::endl;

            std::string msg = _clients_data[id].nickname + " \x1Dleft the server saying '" + message + "'.\x0f\r\n";
            sendToEveryone(msg, id + 1, true);
            removeClient(id + 1);
        }
        else
        {
            std::string msg = _clients_data[id].nickname + " \x1Dleft the server.\x0f\r\n";
            sendToEveryone(msg, id + 1, true);
            removeClient(id + 1);
        }
    }
    return (0);
}
