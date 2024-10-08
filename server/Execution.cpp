/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execution.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:31:34 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/28 22:39:01 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::sendError(const char *command, int id, int value)
{
    std::string  message;

    if (value == ERR_INVALIDCOMMAND)
    {
        message = ": 421 " + std::string(command) + ": Invalid command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_UNKNOWNCOMMAND)
    {
        message = ": 421 " + std::string(command) + ": Unknown or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYPARAMS || value == ERR_NEEDMOREPARAMS || value == ERR_NONICKNAMEGIVEN || value == ERR_KEYSET)
    {
        message = ": 461 " + std::string(command) + ": Invalid number of parameters.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NICKCOLLISION)
    {
        message = ": 433 " + std::string(command) + ": Nickname already taken.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHNICK)
    {
        message = ": 401 " + std::string(command) + ": Member not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHCHANNEL)
    {
        message = ": 403 " + std::string(command) + ": Channel not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANNELISFULL)
    {
        message = ": 471 " + std::string(command) + ": Channel is full.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVITEONLYCHAN)
    {
        message = ": 473 " + std::string(command) + ": Invitation required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_PASSWDMISMATCH)
    {
        message = ": 464 " + std::string(command) + ": Wrong password.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTREGISTERED)
    {
        message = ": 464 " + std::string(command) + ": Authentication required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ERRONEUSNICKNAME)
    {
        message = "Invalid nickname.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTONCHANNEL)
    {
        message = "Presence in channel required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANOPRIVSNEEDED || value == ERR_NOPRIVILEGES)
    {
        message = "Privileges required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTIDENTIFIED)
    {
        message = "Identification required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYREGISTRED)
    {
        message = "Authentication already done.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYINCHANNEL)
    {
        message = "Channel already joined.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYINCHANNEL)
    {
        message = "User already invited.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYCHANNELS)
    {
        message = "Too many channels.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVALIDCHANNELNAME)
    {
        message = "Invalid channel name.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVALIDCHANNELTOPIC)
    {
        message = "Invalid channel topic.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYMODEFLAGS)
    {
        message = "Too many mode flags (flags one by one).\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INACTIVEBOT){
        message = "Bot is turned off.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
}

void    Server::sendToEveryone(std::string message, int id, bool self)
{
    for (int i = 0; i != MAX_CLIENTS; i++)
    {
        if (_clients_data[i].connected == true && _clients_data[i].authentified == true && _clients_data[i].identified == true)
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
}

void    Server::sendToEveryChannelMembers(std::string message, std::string channel, std::string client, bool self)
{
    int i = searchCanal(channel);
    std::vector<std::string>::iterator k;
    for (k = _canals[i].members.begin(); k != _canals[i].members.end(); k++)
    {
        if (self == true)
            send(_sockets_array[searchClient(*k) + 1].fd, message.c_str(), message.size(), 0);
        else if (*k != client)
            send(_sockets_array[searchClient(*k) + 1].fd, message.c_str(), message.size(), 0);
    }
}

int     Server::validateCommandCall(std::string cmd_name, int id) const
{
    if (cmd_name != "NICK" && cmd_name != "USER" && cmd_name != "JOIN" && cmd_name != "PART"
        && cmd_name != "PRIVMSG" && cmd_name != "KICK" && cmd_name != "INVITE"
        && cmd_name != "INVITE" && cmd_name != "TOPIC" && cmd_name != "MODE" && cmd_name != "BOT"
        && cmd_name != "PASS" && cmd_name != "PING" && cmd_name != "PONG" && cmd_name != "QUIT" 
        && cmd_name != "WHO")
    {
        std::cout << getTime() << "Error! " << _clients_data[id - 1].nickname << " failed to request (unknown or unsupported command)." << std::endl;
        return (ERR_UNKNOWNCOMMAND);
    }
    else
    {
        if (isAuthentified(_clients_data[id - 1].nickname) == false)
        {
            if (cmd_name != "PASS" && cmd_name != "QUIT"
                && cmd_name != "PING" && cmd_name != "PONG")
                    return (ERR_NOTREGISTERED);
        }
        if (isIdentified(_clients_data[id - 1].nickname) == false)
        {
            if (cmd_name != "PASS" && cmd_name != "QUIT"
                && cmd_name != "PING" && cmd_name != "PONG"
                && cmd_name != "USER" && cmd_name != "NICK")
                    return (ERR_NOTIDENTIFIED);
        }
    }
    return (0);
}

void    Server::executeCommand(std::string command, std::string cmd_name, int id)
{
    if (cmd_name != "PONG")
        std::cout << getTime() << "Command received from " << _clients_data[id - 1].nickname << " : '" << command << "'." << std::endl;

    int value = validateCommandCall(cmd_name, id);
    if (value == 0)
    {
        if (cmd_name == "PING")
            value = executePingCommand(command, id - 1);
        else if (cmd_name == "PONG")
            value = executePongCommand(command, id - 1);
        else if (cmd_name == "PASS")
            value = executePassCommand(command, id - 1);
        else if (cmd_name == "QUIT")
            value = executeQuitCommand(command, id - 1);
        else if (cmd_name == "NICK")
            value = executeNickCommand(command, id - 1);
        else if (cmd_name == "USER")
            value = executeUserCommand(command, id - 1);
        else if (cmd_name == "JOIN")
            value = executeJoinCommand(command, id - 1);
        else if (cmd_name == "WHO")
            value = executeWhoCommand(command, id - 1);
        else if (cmd_name == "PART")
            value = executePartCommand(command, id - 1);
        else if (cmd_name == "PRIVMSG")
            value = executePrivmsgCommand(command, id - 1);
        else if (cmd_name == "KICK")
            value = executeKickCommand(command, id - 1);
        else if (cmd_name == "INVITE")
            value = executeInviteCommand(command, id - 1);
        else if (cmd_name == "TOPIC")
            value = executeTopicCommand(command, id - 1);
        else if (cmd_name == "MODE")
            value = executeModeCommand(command, id - 1);
        else if (cmd_name == "BOT")
            value = executeBotCommand(command, id - 1);
    }

    if (value != 0)
        sendError(command.c_str(), id, value);
}
