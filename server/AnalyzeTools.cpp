/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnalyzeTools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:17:17 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 23:53:14 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::searchClient(string clientname) const
{
    for (int i = 0; i != SLOTS_LIMIT; i++)
    {
        if (_clients_data[i].nickname == clientname)
            return (i);
    }
    return (-1);
}

int Server::searchCanal(string canal_name) const
{
    for (int i = 0; i != CANALS_LIMIT; i++)
    {
        if (_canals[i].name == canal_name)
            return (i);
    }
    return (-1);
}

