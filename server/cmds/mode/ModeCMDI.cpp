/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMDI.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:02:02 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 02:56:31 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

void    Server::executeModeICommand(int canal_id, int id, char sign)
{
    if (_canals[canal_id].pass_only == true)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to set " << _canals[canal_id].name << "'s access to invite only (already in password only)."<< std::endl;
    }
    else
    {
        if (sign == '+' && _canals[canal_id].invite_only == false)
        {
            _canals[canal_id].invite_only = true;
            std::cout << getTime() << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to invite only." << std::endl;
            std::string message = _clients_data[id].nickname + "\x1D changed\x0f " + _canals[canal_id].name + "\x1D's access to invite only\x0f.\r\n";
            sendToEveryone(message, id + 1, true);
            message = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " +i\r\n";
            sendToEveryChannelMembers(message, _canals[canal_id].name, _clients_data[id].nickname, true);
        }
        if (sign == '-' && _canals[canal_id].invite_only == true)
        {
            _canals[canal_id].invite_only = false;
            std::cout << getTime() << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to public." << std::endl;
            std::string message = _clients_data[id].nickname + "\x1D changed\x0f " + _canals[canal_id].name + "\x1D's access to public\x0f.\r\n";
            sendToEveryone(message, id + 1, true);
            message = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " -i\r\n";
            sendToEveryChannelMembers(message, _canals[canal_id].name, _clients_data[id].nickname, true);
        }
    }
}
