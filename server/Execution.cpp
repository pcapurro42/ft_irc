/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execution.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:31:34 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:56:06 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::sendError(const char *command, int id, int value)
{
    string  message;

    if (value == ERR_INVALIDCOMMAND)
    {
        cout << "Error! " << _clients_data[id - 1].nickname << " failed to request (invalid or unsupported command)." << endl;
        message = "421 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_UNKNOWNCOMMAND)
    {
        message = "421 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYPARAMS || value == ERR_NEEDMOREPARAMS || value == ERR_NONICKNAMEGIVEN || value == ERR_KEYSET)
    {
        message = "461 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid number of parameters.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ERRONEUSNICKNAME)
    {
        message = "433 " + _clients_data[id - 1].nickname + " '" + command + "' :Invalid nickname.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHNICK)
    {
        message = "401 " + _clients_data[id - 1].nickname + " '" + command + "' :Member not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NICKCOLLISION)
    {
        message = "436 " + _clients_data[id - 1].nickname + " '" + command + "' :Nickname already taken.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHCHANNEL)
    {
        message = "403 " + _clients_data[id - 1].nickname + " '" + command + "' :Channel not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANNELISFULL)
    {
        message = "471 " + _clients_data[id - 1].nickname + " '" + command + "' :Channel is full.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVITEONLYCHAN)
    {
        message = "474 " + _clients_data[id - 1].nickname + " '" + command + "' :Invitation required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTONCHANNEL)
    {
        message = "442 " + _clients_data[id - 1].nickname + " '" + command + "' :Presence in channel required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANOPRIVSNEEDED)
    {
        message = "482 " + _clients_data[id - 1].nickname + " '" + command + "' :Privileges required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_PASSWDMISMATCH)
    {
        message = "161 " + _clients_data[id - 1].nickname + " '" + command + "' :Wrong password.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTREGISTERED)
    {
        message = "451 " + _clients_data[id - 1].nickname + " '" + command + "' :Authentication required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYREGISTRED)
    {
        message = "462 " + _clients_data[id - 1].nickname + " '" + command + "' :Authentication already done.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYINCHANNEL)
    {
        message = "477 " + _clients_data[id - 1].nickname + " '" + command + "' :Channel already joined.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOPRIVILEGES)
    {
        message = "440 " + _clients_data[id - 1].nickname + " '" + command + "' :No privileges.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
}

void    Server::sendToEveryone(string message, int id, bool self)
{
    for (int i = 0; i != SLOTS_LIMIT; i++)
    {
        if (self == true)
            send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
        else
        {
            if (i != id)
                send(_sockets_array[i + 1].fd, message.c_str(), message.size(), 0);
        }
    }
}

void    Server::executeCommand(const char *command, string cmd_name, int id)
{
    int value = ERR_INVALIDCOMMAND;
    string cmd(command);

    cout << "Command received from " << _clients_data[id - 1].nickname << " : '" << command << "'." << endl;

    if (cmd_name == "PING")
        value = executePingCommand(cmd, id - 1);
    else if (cmd_name == "PASS")
        value = executePassCommand(cmd, id - 1);
    else if (cmd_name == "NICK")
        value = executeNickCommand(cmd, id - 1);
    else if (cmd_name == "USER")
        value = executeUserCommand(cmd, id - 1);
    else if (cmd_name == "QUIT")
        value = executeQuitCommand(cmd, id - 1);
    else if (cmd_name == "JOIN")
        value = executeJoinCommand(cmd, id - 1);
    else if (cmd_name == "PART")
        value = executePartCommand(cmd, id - 1);
    else if (cmd_name == "PRIVMSG")
        value = executePrivmsgCommand(cmd, id - 1);
    else if (cmd_name == "KICK")
        value = executeKickCommand(cmd, id - 1);
    else if (cmd_name == "INVITE")
        value = executeInviteCommand(cmd, id - 1);
    else if (cmd_name == "TOPIC")
        value = executeTopicCommand(cmd, id - 1);
    else if (cmd_name == "MODE")
        value = executeModeCommand(cmd, id - 1);

    if (value != 0)
        sendError(command, id, value);
}
