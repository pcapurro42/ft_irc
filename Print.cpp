/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 23:41:19 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 17:33:44 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

void    printTitle(void)
{
    cout << "  _____ _____     ___ ____   ____ " << endl;
    cout << " |  ___|_   _|   |_ _|  _ \\ / ___|" << endl;
    cout << " | |_    | |      | || |_) | |    " << endl;
    cout << " |  _|   | |      | ||  _ <| |___ " << endl;
    cout << " |_|     |_|_____|___|_| \\_\\____|" << endl;
    cout << "           |_____|               " << endl;
    cout << endl;
}

int printError(int value)
{
    if (value == -1)
        cerr << "Error! Critical system error." << endl;
    else if (value == 0)
        cerr << "Error! A function failed." << endl;
    else if (value == 1)
    {
        cerr << "Error! Invalid number of arguments." << endl;
        cerr << "Usage: ./ircserv <port> <password>" << endl;
    }
    else if (value == 2)
    {
        cerr << "\rVerifying input... Error!" << endl;
        cerr << "Invalid input." << endl;
        cerr << "Usage: ./ircserv <port> <password>" << endl;
    }
    else if (value == 3)
    {
        cerr << "\rLaunching server... Error!" << endl;
        cerr << "Could not create a socket." << endl;
    }
    else if (value == 4)
    {
        cerr << "\rLaunching server... Error!" << endl;
        cerr << "Could not bind the socket." << endl;
    }
    else if (value == 5)
    {
        cerr << "\rLaunching server... Error!" << endl;
        cerr << "Could not put the socket in listening mod." << endl;
    }
    else if (value == 6)
    {
        cerr << "\rLaunching server... Error!" << endl;
        cerr << "Illegal macro value." << endl;
    }
    return (value);
}

void    printMessage(const char *message, bool end)
{
    string  msg(message);

    for (int i = 0; i != 4; i++)
    {
        cout << "\r" << msg;
        msg = msg + ".";
        cout.flush();
        usleep(100000);
        cout.flush();
    }
    if (end == true)
        cout << endl;
}