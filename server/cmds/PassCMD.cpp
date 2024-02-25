/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:20:42 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:21 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePassCommand(std::string cmd, int id)
{
    std::string password = getArgument(cmd, 1);
    if (password[0] == ':')
        password = password.c_str() + 1;

    if (_clients_data[id].authentified == true)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to authentify (already authentified)." << std::endl;
        return (ERR_ALREADYREGISTRED);
    }
    if (password == _password)
    {
        _clients_data[id].authentified = true;
        std::cout << getTime() << _clients_data[id].nickname << " successfully authentified." << std::endl;

        std::string message = "Password received.\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
    }
    else
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to authentify (incorrect password)." << std::endl;
        return (ERR_PASSWDMISMATCH);
    }
    return (0);
}
