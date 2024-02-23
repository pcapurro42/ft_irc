/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:20:42 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 19:02:33 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePassCommand(string cmd, int id)
{
    string password = getArgument(cmd, 1);
    if (password[0] == ':')
        password = password.c_str() + 1;

    if (_clients_data[id].authentified == true)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to authentify (already authentified)." << endl;
        return (ERR_ALREADYREGISTRED);
    }
    if (password == _password)
    {
        _clients_data[id].authentified = true;
        cout << getTime() << _clients_data[id].nickname << " successfully authentified." << endl;

        string message = "Password received.\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
    }
    else
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to authentify (incorrect password)." << endl;
        return (ERR_PASSWDMISMATCH);
    }
    return (0);
}
