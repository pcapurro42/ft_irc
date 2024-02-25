/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:13:52 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:21 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePingCommand(std::string cmd, int id)
{
    if (std::count(cmd.begin(), cmd.end(), ' ') != 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << std::endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    else
    {
        std::cout << getTime() << "PING received from " << _clients_data[id].nickname << ". Sending PONG..." << std::endl;
 
        std::string message = "PONG " + getArgument(cmd, 1) + "\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
        
        std::cout << getTime() << "PONG sent to " << _clients_data[id].nickname << "." << std::endl;
    }
    return (0);
}
