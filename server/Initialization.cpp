/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Initialization.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:26:55 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 23:28:50 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::initializeServer(void)
{
    _server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_server_socket == -1)
        setFail(), printError(3);
    else
    {
        struct sockaddr_in addr_str;
        addr_str.sin_family = AF_INET;
        addr_str.sin_port = htons(_port);
        addr_str.sin_addr.s_addr = INADDR_ANY;
        for (int i = 0; i != 8; i++)
            addr_str.sin_zero[i] = 0;
 
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

/*

Ce qu'il se passe dans la fonction:
1. Création du socket (l. 18)
2. Association de l'ip (127.0.0.1) et du port (_port) avec le socket (bind, l.29)
3. Mise du socket en mode écoute (l.33) pour être prêt à réceptionner toutes les connexions entrantes

*/