/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:16:07 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:21 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "server/Server.hpp"

bool end_;

void    shutdown(int signal)
{
    end_ = true;
    if (signal == SIGINT)
        std::cout << std::endl << "\033[A^C Ctrl-C used. Shutting down..." << std::endl;
    else if (signal == SIGTERM)
        std::cout << "Terminate order. Shutting down..." << std::endl;
    else if (signal == SIGTSTP)
        std::cout << std::endl << "\033[A^Z Ctrl-Z used. Shutting down..." << std::endl;
}

int main(const int argc, const char **argv)
{
    
    if (argc != 3)
        return (printError(1));
    else if (MAX_CANALS < 1 || MAX_CANALS > 1000 || MAX_CLIENTS < 1 || MAX_CLIENTS > 1000 || BOT < 0 || BOT > 1)
        return (printError(6));
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

            server.LoopRoutine();
        }
    }
    return (0);
}
