/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:20:42 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:45:26 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePassCommand(string cmd, int id)
{
    string password = getArgument(cmd, 1);
    if (password[0] == ':')
        password = password.c_str() + 1;

    if (_clients_data[id - 1].authentified == true)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " failed to authentify (already authentified)." << endl;
        return (ERR_ALREADYREGISTRED);
    }
    if (password == _password)
    {
        _clients_data[id - 1].authentified = true;
        cout << _clients_data[id - 1].nickname << " successfully authentified." << endl;
    }
    else
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " failed to authentify (incorrect password)." << endl;
        return (ERR_PASSWDMISMATCH);
    }
    return (0);
}
