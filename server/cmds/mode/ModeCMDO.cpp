/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMDO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:03:23 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 16:03:58 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

void    Server::executeModeOCommand(std::string member, int canal_id, int id, char sign)
{
    if (sign == '-')
    {
        if (std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member) == _canals[canal_id].operators.end())
            std::cout << getTime() << _clients_data[id].nickname << " failed to remove operator role of " << member << " (not operator)." << std::endl;
        else
        {
            _canals[canal_id].operators.erase(std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member));
            std::cout << getTime() << _clients_data[id].nickname << " removed operator role from " << member << "." << std::endl;
        }
    }
    if (sign == '+')
    {
        if (std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member) == _canals[canal_id].operators.end())
        {
            _canals[canal_id].operators.push_back(member);
            std::cout << getTime() << _clients_data[id].nickname << " gave operator role to " << member << "." << std::endl;
        }
        else
            std::cout << getTime() << _clients_data[id].nickname << " failed to add " << member << " as an operator (already operator)." << std::endl;

    }
}
