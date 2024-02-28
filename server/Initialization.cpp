/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Initialization.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:26:55 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 20:08:40 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::init_clients(void){
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        _clients_data[i].nickname = "";
        _clients_data[i].username = "";
        _clients_data[i].realname = "";
        _clients_data[i].number = 0;
        _clients_data[i].authentified = false;
        _clients_data[i].identified = false;
        _clients_data[i].connected = false;
        _clients_data[i].ping = false;
        _clients_data[i].last_command = "";
        _clients_data[i].set_nickname = false;
        _clients_data[i].set_username = false;
    }
}

void    Server::initializeServer(void)
{
    int value = 1;
    int status_flags = 0;

    _server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_server_socket == -1 || ((status_flags = fcntl(_server_socket, F_GETFL, 0)) == -1) 
        || fcntl(_server_socket, F_SETFL, status_flags | O_NONBLOCK) == -1 
        || setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) == -1)
            setFail(), printError(6);
    else
    {        
        struct sockaddr_in addr_str;    
        addr_str.sin_family = AF_INET;
        addr_str.sin_port = htons(_port);
        addr_str.sin_addr.s_addr = INADDR_ANY;
        for (int i = 0; i != 8; i++)
            addr_str.sin_zero[i] = 0;

        if (bind(_server_socket, reinterpret_cast<sockaddr *>(&addr_str), sizeof(addr_str)) == -1)
            setFail(), printError(7);
        else
        {
            if (listen(_server_socket, SOMAXCONN) == -1)
                setFail(), printError(8);
            else
                std::cout << std::endl << "Server OK!" << std::endl;
        }
    }
}
