/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PongCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:23:03 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 19:02:33 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executePongCommand(string cmd, int id)
{
    if (std::count(cmd.begin(), cmd.end(), ':') != 1)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    else
    {
        _clients_data[id].ping = true;
        cout << getTime() << "PONG received from " << _clients_data[id].nickname << "." << endl;
    }
    return (0);
}
