/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputRectification.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 22:49:09 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 23:27:48 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

vector<string>  Server::rectifyInput(string buffer) const
{
    vector<string>  cmds;
    string          cmd;

    if (std::count(buffer.begin(), buffer.end(), '\n') == 0)
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] >= 32 && buffer[i] <= 126)
                cmd = cmd + buffer[i];
        }
        cmds.push_back(cmd);
    }
    else
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] == '\n')
            {
                cmd = cmd + buffer[i];
                cmds.push_back(cmd);
                cmd.clear();
            }
            else if (buffer[i] >= 32 && buffer[i] <= 126)
                cmd = cmd + buffer[i];
        }
    }
    return (cmds);
}
