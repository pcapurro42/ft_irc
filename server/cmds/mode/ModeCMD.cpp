/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:56 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 02:50:08 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Server.hpp"

int Server::executeModeCommand(std::string cmd, int id)
{
    int value = verifyModeCMD(cmd, id);
    if (value != 0)
        return (value);
    else
    {
        std::string canal = getArgument(cmd, 1);
        int i = searchCanal(canal);

        if (std::count(cmd.begin(), cmd.end(), ' ') == 1)
        {
            std::string flags = "+";
            if (_canals[i].invite_only == true)
                flags = flags + "i";
            if (_canals[i].pass_only == true)
                flags = flags + "k";
            if (_canals[i].op_topic == true)
                flags = flags + "t";
            if (_canals[i].max != MAX_CLIENTS)
                flags = flags + "i";

            std::string message;
            if (flags != "+")
                message = ": 324 " + _clients_data[id].nickname + " " + canal + " " + flags + ".\r\n";
            else
                message = ": 324 " + _clients_data[id].nickname + " " + canal + " [NO MODE]. \r\n";
            send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);
        }

        std::vector<std::string> cmds = splitStringToVector(processModeCMD(cmd));
        std::vector<std::string>::iterator option;
        while (cmds.empty() == false)
        {
            option = cmds.begin();
            char sign = (*option)[0];

            if (*option == "+t" || *option == "-t")
                executeModeTCommand(i, id, sign), cmds.erase(option);
            else if (*option == "+i" || *option == "-i")
                executeModeICommand(i, id, sign), cmds.erase(option);
            else if (*option == "+k" || *option == "-k")
            {
                if (sign == '+')
                {
                    std::vector<std::string>::iterator opt;
                    opt = option;
                    while (((*opt)[0] == '+' || (*opt)[0] == '-') && opt != cmds.end())
                        opt++;
                    std::string pass = *opt;
                    cmds.erase(opt);
                    executeModeKCommand(pass, i, id, sign);
                }
                else
                    executeModeKCommand("", i, id, sign);
                cmds.erase(option);
            }
            else if (*option == "+o" || *option == "-o")
            {
                std::vector<std::string>::iterator opt;
                opt = option;
                while (((*opt)[0] == '+' || (*opt)[0] == '-') && opt != cmds.end())
                    opt++;
                std::string member = *opt;
                cmds.erase(opt);
                executeModeOCommand(member, i, id, sign);
                cmds.erase(option);
            }
            else if (*option == "+l" || *option == "-l")
            {
                if (sign == '+')
                {
                    std::vector<std::string>::iterator opt;
                    opt = option;
                    while (((*opt)[0] == '+' || (*opt)[0] == '-') && opt != cmds.end())
                        opt++;
                    int max = std::atoi((*opt).c_str());
                    cmds.erase(opt);
                    executeModeLCommand(max, i, id, sign);
                }
                else
                    executeModeLCommand(21, i, id, sign);
                cmds.erase(option);
            }
        }
    }
    return (0);
}

// /MODE #canal1 +tkl pass 10 +o shalashaska