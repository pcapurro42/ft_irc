/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMDT.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:02:33 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/27 19:13:48 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

void    Server::executeModeTCommand(int canal_id, int id, char sign)
{
    if (sign == '+' && _canals[canal_id].op_topic == false)
    {
        _canals[canal_id].op_topic = true;
        std::cout << getTime() << _clients_data[id].nickname << " gave " << _canals[canal_id].name << "'s right to modify topic to operators only." << std::endl;
        std::string msg = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " +t\r\n";
        sendToEveryChannelMembers(msg, _canals[canal_id].name, _clients_data[id].nickname, true);
    }
    if (sign == '-' && _canals[canal_id].op_topic == true)
    {
        _canals[canal_id].op_topic = false;
        std::cout << getTime() << _clients_data[id].nickname << " gave " << _canals[canal_id].name << "'s right to modify topic to everyone." << std::endl;
        std::string msg = ":" + _clients_data[id].nickname + " MODE " + _canals[canal_id].name + " -t\r\n";
        sendToEveryChannelMembers(msg, _canals[canal_id].name, _clients_data[id].nickname, true);
    }
}
