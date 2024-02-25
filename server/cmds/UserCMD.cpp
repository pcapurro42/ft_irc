/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:17:40 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/26 00:49:38 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeUserCommand(std::string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 4)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << std::endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 4)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }

    std::string client_username = getArgument(cmd, 1);
    std::string client_realname = getArgument(cmd, 4);

    if (client_realname.empty() == true || client_username.empty() == true || client_realname.size() > 42 || client_username.size() > 42)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to identify (invalid length)." << std::endl;
        return (ERR_UNKNOWNCOMMAND);
    }

    std::string username = getArgument(cmd, 1);
    std::string realname = getArgument(cmd, 4);

    if (realname[0] == ':')
        realname = realname.c_str() + 1;

    for (int i = 0; username[i] != '\0'; i++)
    {
        if (std::isalpha(username[i]) == 0 && std::isdigit(username[i]) == 0)
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to set a username (invalid character)." << std::endl;
            return (ERR_ERRONEUSNICKNAME);
        }
    }
    for (int i = 0; realname[i] != '\0'; i++)
    {
        if (std::isalpha(realname[i]) == 0 && std::isdigit(realname[i]) == 0)
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to set a realname (invalid character)." << std::endl;
            return (ERR_ERRONEUSNICKNAME);
        }
    }

    _clients_data[id].username = username;
    _clients_data[id].realname = realname;

    if (_clients_data[id].set_username == false)
        _clients_data[id].set_username = true;
    
    std::cout << getTime() << _clients_data[id].nickname << " identified as '" << username << "' (username), '" << realname << "' (realname)." << std::endl;
    std::string message = "Username and realname received.\r\n";
    send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);

    if (_clients_data[id].set_nickname == true && _clients_data[id].identified != true)
    {
        _clients_data[id].identified = true;

        sendToEveryone(_clients_data[id].nickname + " \x1Djoined the server\x0f.\r\n", id, false);

        std::string message = "Welcome to our IRC server!\r\n";
        send(_sockets_array[id + 1].fd, message.c_str(), message.size(), 0);

        message = "Welcome dear " + _clients_data[id].nickname + "! I'm Gipiti, the time bot !\r\n";
        std::string msg = ":Gipiti_bot PRIVMSG " + _clients_data[id].nickname + " :" + message;
        send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
        
        message = "Type '!time' to ask for time.\r\n";
        msg = ":Gipiti_bot PRIVMSG " + _clients_data[id].nickname + " :" + message;
        send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);

        message = "Type '!date' to ask for data.\r\n";
        msg = ":Gipiti_bot PRIVMSG " + _clients_data[id].nickname + " :" + message;
        send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);

        message = "Here are the availaible commands: PASS, USER, NICK, KICK, INVITE, JOIN, PING, QUIT, TOPIC AND BOT.\r\n";
        msg = ":Gipiti_bot PRIVMSG " + _clients_data[id].nickname + " :" + message;
        send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);

        return (0);
    }
    return (0);
}
