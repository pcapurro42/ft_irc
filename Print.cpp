/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 23:41:19 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:38:21 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

void    printTitle(void)
{
    std::cout << "  _____ _____     ___ ____   ____ " << std::endl;
    std::cout << " |  ___|_   _|   |_ _|  _ \\ / ___|" << std::endl;
    std::cout << " | |_    | |      | || |_) | |    " << std::endl;
    std::cout << " |  _|   | |      | ||  _ <| |___ " << std::endl;
    std::cout << " |_|     |_|_____|___|_| \\_\\____|" << std::endl;
    std::cout << "           |_____|               " << std::endl;
    std::cout << std::endl;
}

int printError(int value)
{
    if (value == -1)
        std::cerr << "Error! Critical system error." << std::endl;
    else if (value == 0)
        std::cerr << "Error! A function failed." << std::endl;
    else if (value == 1)
    {
        std::cerr << "Error! Invalid number of arguments." << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
    }
    else if (value == 2)
    {
        std::cerr << "\rVerifying input... Error!" << std::endl;
        std::cerr << "Invalid input." << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
    }
    else if (value == 3)
    {
        std::cerr << "\rLaunching server... Error!" << std::endl;
        std::cerr << "Could not create a socket." << std::endl;
    }
    else if (value == 4)
    {
        std::cerr << "\rLaunching server... Error!" << std::endl;
        std::cerr << "Could not bind the socket." << std::endl;
    }
    else if (value == 5)
    {
        std::cerr << "\rLaunching server... Error!" << std::endl;
        std::cerr << "Could not put the socket in listening mod." << std::endl;
    }
    else if (value == 6)
    {
        std::cerr << "\rLaunching server... Error!" << std::endl;
        std::cerr << "Illegal macro value." << std::endl;
    }
    return (value);
}

void    printMessage(const char *message, bool end)
{
    std::string  msg(message);

    for (int i = 0; i != 4; i++)
    {
        std::cout << "\r" << msg;
        msg = msg + ".";
        std::cout.flush();
        usleep(100000);
        std::cout.flush();
    }
    if (end == true)
        std::cout << std::endl;
}