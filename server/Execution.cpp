/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execution.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:31:34 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 00:47:22 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::sendError(const char *command, int id, int value)
{
    std::string  message;

    if (value == ERR_INVALIDCOMMAND)
    {
        std::cout << getTime() << "Error! " << _clients_data[id - 1].nickname << " failed to request (invalid or unsupported command)." << std::endl;
        message = "'" + std::string(command) + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_UNKNOWNCOMMAND)
    {
        message = "'" + std::string(command) + "' :Invalid or unsupported command.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYPARAMS || value == ERR_NEEDMOREPARAMS || value == ERR_NONICKNAMEGIVEN || value == ERR_KEYSET)
    {
        message = "'" + std::string(command) + "' :Invalid number of parameters.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ERRONEUSNICKNAME)
    {
        message = "'" + std::string(command) + "' :Invalid nickname.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHNICK)
    {
        message = "'" + std::string(command) + "' :Member not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NICKCOLLISION)
    {
        message = "'" + std::string(command) + "' :Nickname already taken.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOSUCHCHANNEL)
    {
        message = "'" + std::string(command) + "' :Channel not found.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANNELISFULL)
    {
        message = "'" + std::string(command) + "' :Channel is full.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVITEONLYCHAN)
    {
        message = "'" + std::string(command) + "' :Invitation required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTONCHANNEL)
    {
        message = "'" + std::string(command) + "' :Presence in channel required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CHANOPRIVSNEEDED)
    {
        message = "'" + std::string(command) + "' :Privileges required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_PASSWDMISMATCH)
    {
        message = "'" + std::string(command) + "' :Wrong password.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTREGISTERED)
    {
        message = "'" + std::string(command) + "' :Authentication required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOTIDENTIFIED)
    {
        message = "'" + std::string(command) + "' :Identification required.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYREGISTRED)
    {
        message = "'" + std::string(command) + "' :Authentication already done.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_ALREADYINCHANNEL)
    {
        message = "'" + std::string(command) + "' :Channel already joined.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_NOPRIVILEGES)
    {
        message = "'" + std::string(command) + "' :No privileges.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYCHANNELS)
    {
        message = "'" + std::string(command) + "' :Too many channels.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INVALIDCHANNELNAME)
    {
        message = "'" + std::string(command) + "' :Invalid channel name.(valid: #<name>)\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_TOOMANYMODEFLAGS)
    {
        message = "'" + std::string(command) + "' :Too many mode flags.(use flags one by one)\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_CANNOTSENDTOCHAN){
        message = "'" + std::string(command) + "' :Cannot send to channel.\r\n";
        send(_sockets_array[id].fd, message.c_str(), message.size(), 0);
    }
    else if (value == ERR_INACTIVEBOT){
        message = "'" + std::string(command) + "' :Bot is turned off.\r\n";
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

void    Server::executeCommand(std::string command, std::string cmd_name, int id)
{
    int value = ERR_INVALIDCOMMAND;

    if (cmd_name != "PONG" && cmd_name != "pong")
        std::cout << "Command received from " << _clients_data[id - 1].nickname << " : '" << command << "'." << std::endl;

    if (cmd_name == "PING" || cmd_name == "ping")
        value = executePingCommand(command, id - 1);
    else if (cmd_name == "PONG" || cmd_name == "pong")
        value = executePongCommand(command, id - 1);
    else if (cmd_name == "PASS" || cmd_name == "pass")
        value = executePassCommand(command, id - 1);
    else if (cmd_name == "QUIT" || cmd_name == "quit")
        value = executeQuitCommand(command, id - 1);
    
    if (isAuthentified(_clients_data[id - 1].nickname) == true)
    {
        if (cmd_name == "NICK" || cmd_name == "nick")
            value = executeNickCommand(command, id - 1);
        else if (cmd_name == "USER" || cmd_name == "user")
            value = executeUserCommand(command, id - 1);
    }
    if (isAuthentified(_clients_data[id - 1].nickname) == true && isIdentified(_clients_data[id - 1].nickname) == true)
    {
        if (cmd_name == "JOIN" || cmd_name == "join")
            value = executeJoinCommand(command, id - 1);
        else if (cmd_name == "PART" || cmd_name == "part")
            value = executePartCommand(command, id - 1);
        else if (cmd_name == "PRIVMSG" || cmd_name == "privmsg")
            value = executePrivmsgCommand(command, id - 1);
        else if (cmd_name == "KICK" || cmd_name == "kick")
            value = executeKickCommand(command, id - 1);
        else if (cmd_name == "INVITE" || cmd_name == "invite")
            value = executeInviteCommand(command, id - 1);
        else if (cmd_name == "TOPIC" || cmd_name == "topic")
            value = executeTopicCommand(command, id - 1);
        else if (cmd_name == "MODE" || cmd_name == "mode")
            value = executeModeCommand(command, id - 1);
        else if (cmd_name == "BOT" || cmd_name == "bot")
            value = executeBotCommand(command, id - 1);
    }

    if (value != 0)
        sendError(command.c_str(), id, value);
}
