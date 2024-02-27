/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMDK.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:03:04 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/27 22:26:20 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

void    Server::executeModeKCommand(std::string password, int canal_id, int id, char sign)
{
    if (sign == '+' && password == "")
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to set a channel password (invalid)." << std::endl;
    else
    {
        if (sign == '+' && _canals[canal_id].pass_only == false)
        {
            _canals[canal_id].pass_only = true;
            _canals[canal_id].password = password;
            std::cout << getTime() << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to password only." << std::endl;
            std::string message = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " +k " + password + "\r\n";
            sendToEveryChannelMembers(message, _canals[canal_id].name, _clients_data[id].nickname, true);
        }
        if (sign == '-' && _canals[canal_id].pass_only == true)
        {
            _canals[canal_id].pass_only = false;
            _canals[canal_id].password = "";
            std::cout << getTime() << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to public." << std::endl;
            std::string message = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " -k\r\n";
            sendToEveryChannelMembers(message, _canals[canal_id].name, _clients_data[id].nickname, true);
        }
    }
}
