/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCMD.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:17:27 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/27 19:11:57 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::createCanal(const std::string channels, const std::string nickname)
{
    t_canal canal;
    
    canal.name = channels;
    canal.exist = true;
    canal.members.push_back(nickname);
    canal.operators.push_back(nickname);
    canal.topic = "[NO TOPIC]";
    canal.op_topic = false;
    canal.invite_only = false;
    canal.pass_only = false;
    canal.password = "";
    canal.max = 21;
    
    for (int i = 0; i < MAX_CANALS; i++)
    {
        if (_canals[i].name == "")
        {
            _canals[i] = canal;
            break;
        }
    }
}

int Server::executeJoinCommand(std::string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 2)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << std::endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 1)
    {
        std::cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << std::endl;
        return (ERR_NEEDMOREPARAMS);
    }

    std::string channels = getArgument(cmd, 1);
    std::string passwords = getArgument(cmd, 2);
    std::stringstream ss_channel(channels);
    std::stringstream ss_password(passwords);
    int error = 0;
    while (std::getline(ss_channel, channels, ','))
    {
        error = 0;
        std::getline(ss_password, passwords, ',');
        if (channels[0] != '#')
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join a channel (invalid channel name)." << std::endl;
            error = ERR_INVALIDCHANNELNAME;
        }
        if (error == 0 && searchCanal(channels) == -1 && passwords == "" && space_nb == 1)
        {
            if (_canals[MAX_CANALS - 1].exist == true)
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join a channel (too many channels)." << std::endl;
                error = ERR_TOOMANYCHANNELS;
            }
            else
            {
                createCanal(channels, _clients_data[id].nickname);
                std::cout << getTime() << _clients_data[id].nickname << " created " << channels << "." << std::endl;
                std::cout << getTime() << _clients_data[id].nickname << " joined " << channels << "." << std::endl;
                
                std::string msg = ":" + _clients_data[id].nickname + " JOIN " + channels + " (creation)\r\n";
                send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
                std::string topic = ": 332 " + _clients_data[id].nickname + " " + channels + " :" + _canals[searchCanal(channels)].topic + "\r\n";
                send(_sockets_array[id + 1].fd, topic.c_str(), topic.size(), 0);
                msg = _clients_data[id].nickname + " \x1Dhas created\x0f " + channels + ".\r\n";
                sendToEveryone(msg, id + 1, true);
                msg = _clients_data[id].nickname + " \x1Dhas joined\x0f " + channels + ".\r\n";
                sendToEveryone(msg, id + 1, true);
                sendNames(channels);  
                continue;
            }
        }
        if (error == 0 && searchCanal(channels) == -1 && passwords != "")
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join a channel (does not exist)." << std::endl;
            error = ERR_NOSUCHCHANNEL;
        }
        if (error == 0 && _canals[searchCanal(channels)].members.size() >= static_cast<size_t>(_canals[searchCanal(channels)].max)) 
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (channel is full)." << std::endl;
            error = ERR_CHANNELISFULL;
        }
        if (error == 0){
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].invited.begin(), _canals[searchCanal(channels)].invited.end(), _clients_data[id].nickname);
            if (_canals[searchCanal(channels)].invite_only == true && it == _canals[searchCanal(channels)].invited.end())
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (invite only)." << std::endl;
                error = ERR_INVITEONLYCHAN;
            }
        }
        if (error == 0){
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].members.begin(), _canals[searchCanal(channels)].members.end(), _clients_data[id].nickname);
            if (it != _canals[searchCanal(channels)].members.end())
            {
                std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (already in channel)." << std::endl;
                error = ERR_ALREADYINCHANNEL;
            }
        }
        if (error == 0 && _canals[searchCanal(channels)].pass_only == true && _canals[searchCanal(channels)].password != passwords)
        {
            std::cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to join " + channels + " (incorrect password)." << std::endl;
            error = ERR_BADCHANNELKEY;
        }
        
        if (error == 0)
        {
            _canals[searchCanal(channels)].members.push_back(_clients_data[id].nickname);
            std::cout << getTime() << _clients_data[id].nickname << " has joined " << channels << "." << std::endl;
            
            std::string msg = ":" + _clients_data[id].nickname + " JOIN " + channels + "\r\n";
            send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
            
            msg = ":" + _clients_data[id].nickname + " JOIN " + channels + "\r\n";
            sendToEveryChannelMembers(msg, channels, _clients_data[id].nickname, true);

            std::string topic = ": 332 " + _clients_data[id].nickname + " " + channels + " :" + _canals[searchCanal(channels)].topic + "\r\n";
            send(_sockets_array[id + 1].fd, topic.c_str(), topic.size(), 0);

            sendToEveryone(_clients_data[id].nickname + " \x1Dhas joined\x0f " + channels + ".\r\n", id, true);
            sendNames(channels);
        }
        else
            sendError(std::string("JOIN " + channels).c_str(), id + 1, error);
    }
    return (0);
}


void Server::sendNames(std::string channel){
    std::vector<std::string> members = _canals[searchCanal(channel)].members;
    std::vector<std::string> ops = _canals[searchCanal(channel)].operators;
    std::string names;
    
    for (size_t i = 0; i < members.size(); ++i){
        if (isOp(members[i], channel))
            names += "@" + members[i] + " ";
        else
            names += members[i] + " ";

    }
    
    int i = searchCanal(channel);
    std::vector<std::string>::iterator k;
    for (k = _canals[i].members.begin(); k != _canals[i].members.end(); k++){
        std::string msg = ":server 353 " + *k + " = " + channel + " :" + names + "\r\n";
        send(_sockets_array[searchClient(*k) + 1].fd, msg.c_str(), msg.size(), 0);
        msg = ":server 366 " + *k + " " + channel + " :End of /NAMES list\r\n";
        send(_sockets_array[searchClient(*k) + 1].fd, msg.c_str(), msg.size(), 0);
    }

}