/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMDL.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:04:10 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 16:04:22 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

void    Server::executeModeLCommand(int value, int canal_id, int id)
{
    if (value > MAX_CLIENTS || value <= 0 || value < static_cast<int>(_canals[canal_id].members.size()))
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to change the limit value of " << _canals[canal_id].name << " (invalid value)." << std::endl;
    else
    {
        _canals[canal_id].max = value;
        std::cout << getTime() << _clients_data[id].nickname << " changed the limit value of " << _canals[canal_id].name << "." << std::endl;
        std::string message = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " +l " + convertNumberToString(value) + "\r\n";
        sendToEveryone(message, id + 1, true);
    }
}
