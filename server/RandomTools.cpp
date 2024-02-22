/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RandomTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 01:50:37 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 13:36:33 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::validateInput(const int port_nb, const char *password) const
{
    if (port_nb < 0 || port_nb > 65535)
        return (false);
    if (password[0] == '\0')
        return (false);
    
    cout << "\rVerifying inputs... OK!" << endl;
    return (true);
}

string  Server::getArgument(const string cmd, int nb_arg) const
{
    int i = 0;
    int count = 0;
    string argument("");
    
    if (nb_arg > std::count(cmd.begin(), cmd.end(), ' '))
        return (string(""));
    else
    {
        while (cmd[i] != '\0')
        {
            if (count == nb_arg && cmd[i] != ' ')
                argument = argument + cmd[i];
            if (cmd[i] == ' ')
                count++;
            i++;
        }
    }
    return (argument);
}

string  Server::convertNumberToString(int value) const
{
    int nb = value;
    int i = 0;

    while (nb > 9)
        nb = nb / 10, i++;
    i = i + 1;
    
    char buffer[i + 1];
    buffer[i] = '\0';

    nb = value;
    i = i - 1;
    
    while (nb > 9)
    {
        buffer[i] = (nb % 10) + 48;
        nb = nb / 10;
        i--;
    }
    buffer[0] = (nb % 10) + 48;
    
    return (string(buffer));
}

std::string Server::get_time(void) const{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string str = asctime(timeinfo);
    str.resize(str.size() - 1); //suppression du \n
    return (str);
}
