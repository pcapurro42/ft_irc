/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:56 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 03:13:56 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::verifyModeCMD(std::string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), '#') == 0 || (std::count(cmd.begin(), cmd.end(), '+') == 0 && std::count(cmd.begin(), cmd.end(), '-') == 0) || std::count(cmd.begin(), cmd.end(), '*') != 0)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " typed an invalid or unsupported command." << std::endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    else if (std::count(cmd.begin(), cmd.end(), ' ') < 2)
    {
        std::cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }
    return (0);
}

void    Server::executeModeICommand(int canal_id, int id, char sign)
{
    if (sign == '+' && _canals[canal_id].invite_only == false)
    {
        _canals[canal_id].invite_only = true;
        std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to invite only." << std::endl;
        std::string message = _clients_data[id].nickname + " \x1D\\changed\x0f " + _canals[canal_id].name + "\x1D's access to invite only\x0f.\r\n";
        sendToEveryone(message, id + 1, true);
    }
    if (sign == '-' && _canals[canal_id].invite_only == true)
    {
        _canals[canal_id].invite_only = false;
        std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to public." << std::endl;
    }
}

void    Server::executeModeTCommand(int canal_id, int id, char sign)
{
    if (sign == '+' && _canals[canal_id].op_topic == false)
    {
        _canals[canal_id].op_topic = true;
        std::cout << _clients_data[id].nickname << " gave " << _canals[canal_id].name << "'s right to modify topic to operators only." << std::endl;
    }
    if (sign == '-' && _canals[canal_id].op_topic == true)
    {
        _canals[canal_id].op_topic = false;
        std::cout << _clients_data[id].nickname << " gave " << _canals[canal_id].name << "'s right to modify topic to everyone." << std::endl;
    }
}

void    Server::executeModeKCommand(std::string password, int canal_id, int id, char sign)
{
    if (password == "")
        std::cout << "Error! " << _clients_data[id].nickname << " set an invalid password." << std::endl;
    else
    {
        if (sign == '+' && _canals[canal_id].pass_only == false)
        {
            _canals[canal_id].pass_only = true;
            _canals[canal_id].password = password;
            std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to password only." << std::endl;
        }
        if (sign == '-' && _canals[canal_id].pass_only == true)
        {
            _canals[canal_id].pass_only = false;
            _canals[canal_id].password = "";
            std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to public." << std::endl;
        }
    }
}

void    Server::executeModeOCommand(std::string member, int canal_id, int id, char sign)
{
    if (sign == '-')
    {
        if (std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member) == _canals[canal_id].operators.end())
            std::cout << _clients_data[id].nickname << " failed to remove operator role of " << member << " (not operator)." << std::endl;
        else
            _canals[canal_id].operators.erase(std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member));
    }
    if (sign == '+')
    {
        if (std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member) == _canals[canal_id].operators.end())
            _canals[canal_id].operators.push_back(member);
        else
            std::cout << _clients_data[id].nickname << " failed to add " << member << " as an operator (already operator)." << std::endl;

    }
}

void    Server::executeModeLCommand(int value, int canal_id, int id)
{
    if (value > MAX_CLIENTS || value <= 0 || value < static_cast<int>(_canals[canal_id].members.size()))
        std::cout << "Error! " << _clients_data[id].nickname << " failed to change the limit value of " << _canals[canal_id].name << " (invalid value)." << std::endl;
    else
    {
        _canals[canal_id].max = value;
        std::cout << _clients_data[id].nickname << " changed the limit value of " << _canals[canal_id].name << "." << std::endl;
    }
}

int Server::executeModeCommand(std::string cmd, int id)
{
    int value = verifyModeCMD(cmd, id);
    if (value != 0)
        return (value);
    else
    {
        std::string canal = getArgument(cmd, 1);
        int i = searchCanal(canal);
        if (i == -1)
        {
            std::cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << std::endl;
            return (ERR_NOSUCHCHANNEL);
        }
        else if (std::find(_canals[i].members.begin(), _canals[i].members.end(), _clients_data[id].nickname) == _canals[i].members.end())
        {
            std::cout << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (not member)." << std::endl;
            return (ERR_NOPRIVILEGES);
        }
        else if (std::find(_canals[i].operators.begin(), _canals[i].operators.end(), _clients_data[id].nickname) == _canals[i].operators.end())
        {
            std::cout << "Error! " << _clients_data[id].nickname << " failed to mode in " << _canals[i].name << " (not member)." << std::endl;
            return (ERR_NOPRIVILEGES);
        }

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

        std::vector<std::string> cmds = splitStringToVector(real_cmd);
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
                std::vector<std::string>::iterator opt;
                opt = option;
                while (((*opt)[0] == '+' || (*opt)[0] == '-') && opt != cmds.end())
                    opt++;
                std::string pass = *opt;
                cmds.erase(opt);
                executeModeKCommand(pass, i, id, sign);
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

        // /MODE #canal1 +tkl pass 10 +o shalashaska
        // '+k +l +t +o pass 10 +o shalashaska
        // '+k+l+t+opass10+oshalashaska
        // '+t' ; '+o' '+l' ; '+k' ; 'pass' ; '10' ; '+o' ; 'shalashaska'
    }
    return (0);
}
