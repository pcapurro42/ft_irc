/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RandomTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 01:50:37 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:53:25 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::validateInput(const int port_nb, const char *password) const
{
    if (port_nb <= 1023 || port_nb > 65535)
        return (false);

    if (std::string(password).size() > 42)
        return (false);
    else if (password[0] == '\0')
        return (false);
    for (int i = 0; password[i] != '\0'; i++)
    {
        if (password[i] <= 32 || password[i] >= 127)
            return (false);
    }
    
    std::cout << "\rVerifying inputs... OK!" << std::endl;
    return (true);
}

std::string  Server::getArgument(const std::string cmd, int nb_arg) const
{
    int i = 0;
    int count = 0;
    std::string argument("");
    
    if (nb_arg > std::count(cmd.begin(), cmd.end(), ' '))
        return (std::string(""));
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

std::string  Server::convertNumberToString(int value) const
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
    
    return (std::string(buffer));
}

std::string Server::getTime(void) const{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string str = asctime(timeinfo);
    str.resize(str.size() - 1);
    return (std::string("<" + getArgument(str, 3) + "> "));
}
