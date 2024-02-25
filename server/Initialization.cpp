/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Initialization.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:26:55 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 17:11:24 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::initializeServer(void)
{
    int value = 1;

    _server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    value = setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));
    if (_server_socket == -1 || value == -1)
        setFail(), printError(3);
    else
    {
        int enable;
        struct sockaddr_in addr_str;
        addr_str.sin_family = AF_INET;
        addr_str.sin_port = htons(_port);
        addr_str.sin_addr.s_addr = INADDR_ANY;
        for (int i = 0; i != 8; i++)
            addr_str.sin_zero[i] = 0;
        setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if (bind(_server_socket, reinterpret_cast<sockaddr *>(&addr_str), sizeof(addr_str)) == -1)
            setFail(), printError(4);
        else
        {
            if (listen(_server_socket, SOMAXCONN) == -1)
                setFail(), printError(5);
            else
                cout << endl << "Server OK!" << endl;
        }
    }
}
