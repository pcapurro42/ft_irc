/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:56 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 16:06:58 by pcapurro         ###   ########.fr       */
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
                std::vector<std::string>::iterator opt;
                opt = option;
                while (((*opt)[0] == '+' || (*opt)[0] == '-') && opt != cmds.end())
                    opt++;
                int max = std::atoi((*opt).c_str());
                cmds.erase(opt);
                executeModeLCommand(max, i, id);
                cmds.erase(option);
            }
        }
    }
    return (0);
}

// /MODE #canal1 +tkl pass 10 +o shalashaska