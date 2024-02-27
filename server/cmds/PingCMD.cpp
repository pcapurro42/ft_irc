/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:13:52 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 00:20:10 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePingCommand(std::string cmd, int id)
{
    std::cout << getTime() << "PING received from " << _clients_data[id].nickname << ". Sending PONG..." << std::endl;

    std::string message;
    if (getArgument(cmd, 1) != "")
        message = "PONG " + getArgument(cmd, 1) + "\r\n";
    else
        message = "PONG\r\n";
    send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);    
    
    std::cout << getTime() << "PONG sent to " << _clients_data[id].nickname << "." << std::endl;
    
    return (0);
}
