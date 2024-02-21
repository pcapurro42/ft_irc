/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:17:40 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/21 11:39:57 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeUserCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 4)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 4)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }

    string client_username = getArgument(cmd, 1);
    string client_realname = getArgument(cmd, 4);

    if (client_realname.empty() == true || client_username.empty() == true || client_realname.size() > 42 || client_username.size() > 42)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to identify (invalid length)." << endl;
        return (ERR_UNKNOWNCOMMAND);
    }

    if (_clients_data[id].authentified == false)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }

    string username = getArgument(cmd, 1);
    string realname = getArgument(cmd, 4);

    if (realname[0] == ':')
        realname = realname.c_str() + 1;
    _clients_data[id].username = username;
    _clients_data[id].realname = realname;

    cout << _clients_data[id].nickname << " identified as '" << username << "' (username), '" << realname << "' (realname)." << endl;

    if (_clients_data[id].nickname.empty() != true && _clients_data[id].identified != true)
    {
        _clients_data[id].identified = true;

        sendToEveryone(": " + _clients_data[id].nickname + " \x1Djoined the server.\x0f\r\n", id, true);

        string message = "001 " + _clients_data[id].nickname + " :\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
            
        return (0);
    }
    return (0);
}
