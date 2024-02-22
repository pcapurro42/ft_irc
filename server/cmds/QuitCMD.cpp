/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:20:32 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 14:46:12 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeQuitCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 1)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else
    {
        if (std::count(cmd.begin(), cmd.end(), ' ') != 0)
        {
            string message = getArgument(cmd, 1);
            if (message[0] == ':')
                message = message.c_str() + 1;

            cout << _clients_data[id].nickname << " left the server saying '" << message << "' (" << _clients_slots - 1 << "/" << MAX_CLIENTS << ")." << endl;
            removeClient(id + 1, 2);
        }
        else
            removeClient(id + 1, 0);
    }
    return (0);
}
