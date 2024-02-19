/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:16:07 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:57:06 by pcapurro         ###   ########.fr       */
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

        if (server.fail() == true)
            return (1);
        else
        {
            end_ = false;
            signal(SIGINT, shutdown);
            signal(SIGTERM, shutdown);
            signal(SIGTSTP, shutdown);

            server.startLoopRoutine();
        }
    }
    return (0);
}

// ./ircserv <port> <password>

// ports TCP (transmission control protocol)
// -> 0 à 65535
