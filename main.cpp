/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:16:07 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/23 17:26:32 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "server/Server.hpp"

bool end_;

void    shutdown(int signal)
{
    end_ = true;
    if (signal == SIGINT)
        cout << endl << "\033[A^C Ctrl-C used. Shutting down..." << endl;
    else if (signal == SIGTERM)
        cout << "Terminate order. Shutting down..." << endl;
    else if (signal == SIGTSTP)
        cout << endl << "\033[A^Z Ctrl-Z used. Shutting down..." << endl;
    
    exit(0); // temporaire mais indispensable pour l'instant
}

int main(const int argc, const char **argv)
{
    if (argc != 3)
        return (printError(1));
    else
    {
        printTitle();

        Server server(atoi(argv[1]), argv[2]);

        if (server.fail() == true) // vérif si le serveur a validé les args + a réussi à créer le point d'accès, etc
            return (1);
        else
        {
            end_ = false;
            signal(SIGINT, shutdown);
            signal(SIGTERM, shutdown);
            signal(SIGTSTP, shutdown);
            signal(SIGPIPE, SIG_IGN); // fix server qui s'arrete (situation trouve: join un canal inexistant depuis le premier user de hexchat, pas de probleme avec n-ieme user)

            server.startLoopRoutine(); // démarrage de la boucle de routine
        }
    }
    return (0);
}

// ./ircserv <port> <password>

// ports TCP (transmission control protocol)
// -> 0 à 65535
