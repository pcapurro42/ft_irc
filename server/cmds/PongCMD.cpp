/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:23:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 00:32:51 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePongCommand(std::string cmd, int id)
{
    (void) cmd;
    _clients_data[id].ping = true;
    // std::cout << getTime() << "PONG received from " << _clients_data[id].nickname << "." << std::endl;
    return (0);
}
