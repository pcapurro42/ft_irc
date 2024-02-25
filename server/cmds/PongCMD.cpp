/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:23:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:21 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePongCommand(std::string cmd, int id)
{
    if (std::count(cmd.begin(), cmd.end(), ':') != 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << std::endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    else
    {
        _clients_data[id].ping = true;
        // std::cout << getTime() << "PONG received from " << _clients_data[id].nickname << "." << std::endl;
    }
    return (0);
}
