/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SearchTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 17:57:09 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 17:57:22 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::searchClient(string clientname) const
{
    if (clientname == "Gipiti_bot" && BOT == 1)
        return (0);
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].nickname == clientname)
            return (i);
    }
    return (-1);
}

int Server::searchCanal(string canal_name) const
{
    for (int i = 0; i != MAX_CANALS; i++)
    {
        if (_canals[i].name == canal_name)
            return (i);
    }
    return (-1);
}
