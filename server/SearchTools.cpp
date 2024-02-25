/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SearchTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 17:57:09 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 00:43:54 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::searchClient(std::string clientname) const
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

int Server::searchCanal(std::string canal_name) const
{
    for (int i = 0; i != MAX_CANALS; i++)
    {
        if (_canals[i].name == canal_name)
            return (i);
    }
    return (-1);
}

bool    Server::isAuthentified(std::string clientname) const
{
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].nickname == clientname)
        {
            if (_clients_data[i].authentified == true)
                return (true);
            break ;
        }
    }
    return (false);
}

bool    Server::isIdentified(std::string clientname) const
{
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].nickname == clientname)
        {
            if (_clients_data[i].identified == true)
                return (true);
            break ;
        }
    }
    return (false);
}
