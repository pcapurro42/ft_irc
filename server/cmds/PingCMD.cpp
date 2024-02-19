/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:13:52 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:54:46 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePingCommand(string cmd, int id)
{
    if (std::count(cmd.begin(), cmd.end(), ' ') != 1)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    else
    {
        cout << "PING received from " << _clients_data[id].nickname << ". Sending PONG..." << endl;
 
        string message = "PONG " + getArgument(cmd, 1) + "\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
        
        cout << "PONG sent to " << _clients_data[id].nickname << "." << endl;
    }
    return (0);
}
