/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:56 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 00:48:22 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::verifyModeCMD(std::string cmd, int id) const
{
    if (std::count(cmd.begin(), cmd.end(), '#') == 0 || std::count(cmd.begin(), cmd.end(), '+') == 0)
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

void    Server::executeModeICommand(int canal_id, int id)
{
    if (_canals[canal_id].invite_only == false)
    {
        _canals[canal_id].invite_only = true;
        std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to invite only." << std::endl;
        std::string message = _clients_data[id].nickname + " \x1D\\changed\x0f " + _canals[canal_id].name + "\x1D's access to invite only\x0f.\r\n";
        sendToEveryone(message, id + 1, true);
    }
    else
    {
        _canals[canal_id].invite_only = false;
        std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to public." << std::endl;
    }
}

void    Server::executeModeTCommand(int canal_id, int id)
{
    if (_canals[canal_id].op_topic == true)
    {
        _canals[canal_id].op_topic = false;
        std::cout << _clients_data[id].nickname << " gave " << _canals[canal_id].name << "'s right to modify topic to everyone." << std::endl;
    }
    else
    {
        _canals[canal_id].op_topic = true;
        std::cout << _clients_data[id].nickname << " gave " << _canals[canal_id].name << "'s right to modify topic to operators only." << std::endl;
    }
}

void    Server::executeModeKCommand(std::string password, int canal_id, int id)
{
    if (_canals[canal_id].pass_only == false)
    {
        _canals[canal_id].pass_only = true;
        _canals[canal_id].password = password;
        std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to password only." << std::endl;
    }
    else
    {
        _canals[canal_id].pass_only = false;
        _canals[canal_id].password = "";
        std::cout << _clients_data[id].nickname << " changed " << _canals[canal_id].name << "'s access to public." << std::endl;
    }
}

void    Server::executeModeOCommand(std::string member, int canal_id, int id)
{
    if (searchClient(member) == -1)
        std::cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent user." << std::endl;
    else
    {
        if (std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member) == _canals[canal_id].operators.end())
        {
            _canals[canal_id].operators.push_back(member);
            std::cout << _clients_data[id].nickname << " added " << member << " as an operator of " << _canals[canal_id].name << "." << std::endl;
        }
        else
        {
            _canals[canal_id].operators.erase(std::find(_canals[canal_id].operators.begin(), _canals[canal_id].operators.end(), member));
            std::cout << _clients_data[id].nickname << " removed " << member << " as an operator of " << _canals[canal_id].name << "." << std::endl;
        }
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
            if (new_cmd[k] == '+')
            {
                k++;
                while (new_cmd[k] != ' ' && new_cmd[k] != '\0')
                {
                    real_cmd = real_cmd + "+" + new_cmd[k] + " ";
                    k++;
                }
            }
            else
                real_cmd = real_cmd + new_cmd[k];
        }

        for (int k = 0; real_cmd[k] != '\0'; k++)
        {
            if (real_cmd[k] == '+')
            {
                if (real_cmd[k + 1] != 't' && real_cmd[k + 1] != 'l' && real_cmd[k + 1] != 'k' \
                && real_cmd[k + 1] != 'o' && real_cmd[k + 1] != 'i' && real_cmd[k + 1] != ' ' && real_cmd[k + 1] != '\0')
                {
                    std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid paramater)." << std::endl;
                    return (ERR_UNKNOWNCOMMAND);
                }
                    
            }
        }

        int icount, tcount = 0;
        int kcount, ocount = 0;
        int lcount = 0;
        for (int k = 0; real_cmd[k] != '\0'; k++)
        {
            if (real_cmd[k] == '+')
            {
                if (real_cmd[k + 1] == 'i')
                    icount++;
                else if (real_cmd[k + 1] == 'k')
                    kcount++;
                else if (real_cmd[k + 1] == 't')
                    tcount++;
                else if (real_cmd[k + 1] == 'o')
                    ocount++;
                else if (real_cmd[k + 1] == 'l')
                    lcount++;
            }
        }
        if (icount > 1 || tcount > 1 \
        || kcount > 1 || ocount > 1 || lcount > 1)
        {
            std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid number of paramaters)." << std::endl;
            return (ERR_UNKNOWNCOMMAND);
        }

        std::string arg = "42";
        for (int k = 0; arg != ""; k++)
        {
            arg = getArgument(real_cmd, k);
            if (arg == "+i")
            {
                k++;
                if (getArgument(real_cmd, k)[0] != '+' && getArgument(real_cmd, k) != "")
                {
                    std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid number of arguments for a paramater)." << std::endl;
                    return (ERR_UNKNOWNCOMMAND);
                }
                k--;
            }
            else if (arg == "+t")
            {
                k++;
                if (getArgument(real_cmd, k)[0] != '+' && getArgument(real_cmd, k) != "")
                {
                    std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid number of arguments for a paramater)." << std::endl;
                    return (ERR_UNKNOWNCOMMAND);
                }
                k--;
            }
            else if (arg == "+k")
            {
                k = k + 2;
                if (getArgument(real_cmd, k)[0] != '+' && getArgument(real_cmd, k) != "")
                {
                    std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid number of arguments for a paramater)." << std::endl;
                    return (ERR_UNKNOWNCOMMAND);
                }
                k = k - 2;
            }
            else if (arg == "+o")
            {
                k = k + 2;
                if (getArgument(real_cmd, k)[0] != '+' && getArgument(real_cmd, k) != "")
                {
                    std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid number of arguments for a paramater)." << std::endl;
                    return (ERR_UNKNOWNCOMMAND);
                }
                k = k - 2;
            }
            else if (arg == "+l")
            {
                k = k + 2;
                if (getArgument(real_cmd, k)[0] != '+')
                {
                    std::cout << "Error! " << _clients_data[id].nickname << " failed to use MODE in " << _canals[i].name << " (invalid number of arguments for a paramater)." << std::endl;
                    return (ERR_UNKNOWNCOMMAND);
                }
                k = k - 2;
            }
        }

        arg = "42";
        for (int k = 0; arg != ""; k++)
        {
            arg = getArgument(real_cmd, k);
            if (arg == "+i")
                executeModeICommand(i, id);
            else if (arg == "+t")
                executeModeTCommand(i, id);
            else if (arg == "+k")
            {
                k = k + 1;
                std::string password = getArgument(real_cmd, k);
                executeModeKCommand(password, i, id);
            }
            else if (arg == "+o")
            {
                k = k + 1;
                std::string member = getArgument(real_cmd, k);
                executeModeOCommand(member, i, id);
            }
            else if (arg == "+l")
            {
                k = k + 1;
                int value = std::atoi(getArgument(real_cmd, k).c_str());
                executeModeLCommand(value, i, id);
            }
        }
    }
    return (0);
}
