/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMDVerify.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:58:31 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 02:38:35 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

std::string Server::processModeCMD(std::string cmd) const
{
    std::string new_cmd;
    for (int k = 2; getArgument(cmd, k) != ""; k++)
        new_cmd = new_cmd + getArgument(cmd, k) + " ";

    std::string real_cmd;
    for (int k = 0; new_cmd[k] != '\0'; k++)
    {
        if (new_cmd[k] == '+' || new_cmd[k] == '-')
        {
            char sign = new_cmd[k];
            k++;
            while (new_cmd[k] != ' ' && new_cmd[k] != '\0')
            {
                if (sign == '+')
                    real_cmd = real_cmd + "+" + new_cmd[k] + " ";
                else if (sign == '-')
                    real_cmd = real_cmd + "-" + new_cmd[k] + " ";
                k++;
            }
        }
        else
            real_cmd = real_cmd + new_cmd[k];
    }
    return (real_cmd);
}

int Server::verifyModeCMD(std::string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), ' ') == 1)
    {
        if (getArgument(cmd, 1)[0] != '#')
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid command." << std::endl;
            return (ERR_INVALIDCOMMAND);
        }
        if (searchCanal(getArgument(cmd, 1)) == -1)
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << std::endl;
            return (ERR_NOSUCHCHANNEL);
        }
            return (0);
    }
    
    if (std::count(cmd.begin(), cmd.end(), '#') == 0 || (std::count(cmd.begin(), cmd.end(), '+') == 0 && std::count(cmd.begin(), cmd.end(), '-') == 0) || std::count(cmd.begin(), cmd.end(), '*') != 0)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed an invalid command." << std::endl;
        return (ERR_INVALIDCOMMAND);
    }
    else if (std::count(cmd.begin(), cmd.end(), ' ') < 2)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }

    std::string canal = getArgument(cmd, 1);
    int i = searchCanal(canal);

    if (i == -1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << std::endl;
        return (ERR_NOSUCHCHANNEL);
    }
    else if (std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id].nickname) == _canals[i].members.end())
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (not member)." << std::endl;
        return (ERR_NOPRIVILEGES);
    }
    else if (std::find(_canals[i].operators.begin(), _canals[i].operators.end(), _clients_data[id].nickname) == _canals[i].operators.end())
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (not member)." << std::endl;
        return (ERR_NOPRIVILEGES);
    }

    std::string real_cmd = processModeCMD(cmd);

    for (int k = 0; real_cmd[k] != '\0'; k++)
    {
        if (real_cmd[k] == '+' || real_cmd[k] == '-')
        {
            if (real_cmd[k + 1] == ' ' || real_cmd[k + 1] == '\0')
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (inconsistent use)." << std::endl;
                return (ERR_NOPRIVILEGES);
            }
            else if (real_cmd[k + 1] != 't' && real_cmd[k + 1] != 'i' && real_cmd[k + 1] != 'k' 
                && real_cmd[k + 1] != 'l' && real_cmd[k + 1] != 'o')
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (inconsistent use)." << std::endl;
                return (ERR_NOPRIVILEGES);
            }
        }
    }

    std::vector<std::string> verify_cmds = splitStringToVector(real_cmd);
    while (std::find(verify_cmds.begin(), verify_cmds.end(), "+t") != verify_cmds.end())
        verify_cmds.erase(std::find(verify_cmds.begin(), verify_cmds.end(), "+t"));
    while (std::find(verify_cmds.begin(), verify_cmds.end(), "-t") != verify_cmds.end())
        verify_cmds.erase(std::find(verify_cmds.begin(), verify_cmds.end(), "-t"));
    while (std::find(verify_cmds.begin(), verify_cmds.end(), "+i") != verify_cmds.end())
        verify_cmds.erase(std::find(verify_cmds.begin(), verify_cmds.end(), "+i"));
    while (std::find(verify_cmds.begin(), verify_cmds.end(), "-i") != verify_cmds.end())
        verify_cmds.erase(std::find(verify_cmds.begin(), verify_cmds.end(), "-i"));
    while (std::find(verify_cmds.begin(), verify_cmds.end(), "-k") != verify_cmds.end())
        verify_cmds.erase(std::find(verify_cmds.begin(), verify_cmds.end(), "-k"));
    while (std::find(verify_cmds.begin(), verify_cmds.end(), "-l") != verify_cmds.end())
        verify_cmds.erase(std::find(verify_cmds.begin(), verify_cmds.end(), "-l"));
        
    if (verify_cmds.size() % 2 != 0)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (inconsistent use)." << std::endl;
        return (ERR_NOPRIVILEGES);
    }
    return (0);
}
