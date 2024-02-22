/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputRectification.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 22:49:09 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 23:19:55 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

vector<string>  Server::rectifyInput(const char *buffer) const
{
    vector<string>  cmds;
    string          cmd;

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '\n')
        {
            cmds.push_back(string(cmd));
            cmd.clear();
        }
        else if (buffer[i] >= 32 && buffer[i] <= 126)
            cmd = cmd + buffer[i];
    }
    return (cmds);
}
