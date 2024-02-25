/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCMD.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:22:43 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/25 19:08:27 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int Server::executeTopicCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');
    if (space_nb > 2)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 1)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }
    if (_clients_data[id].authentified != true)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified != true)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << endl;
        return (ERR_NOTIDENTIFIED);
    }
    std::string channels = getArgument(cmd, 1);
    std::string topic = getArgument(cmd, 2);
    if (searchCanal(channels) == -1)
    {
        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (canal not found)." << endl;
        return (ERR_NOSUCHCHANNEL);
    }
    else if (searchCanal(channels) != -1){
        if (getArgument(cmd, 2) == "")
        {
            cout << getTime() << _clients_data[id].nickname << " requested the topic of " << channels << "." << endl;
            std::string msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + " :" + _canals[searchCanal(channels)].topic + "\r\n";
            if (_canals[searchCanal(channels)].topic == "")
                msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + " :No topic is set\r\n";
            send(_sockets_array[id + 1].fd, msg.c_str(), msg.size(), 0);
        }
        else
        {
            std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].members.begin(), _canals[searchCanal(channels)].members.end(), _clients_data[id].nickname);
            if (it != _canals[searchCanal(channels)].members.end())
            {
                if (_canals[searchCanal(channels)].op_topic == true)
                {
                    std::vector<std::string>::iterator it = std::find(_canals[searchCanal(channels)].operators.begin(), _canals[searchCanal(channels)].operators.end(), _clients_data[id].nickname);
                    if (it != _canals[searchCanal(channels)].operators.end())
                    {
                        _canals[searchCanal(channels)].topic = topic;
                        cout << getTime() << _clients_data[id].nickname << " changed the topic of " << channels << " to " << topic << "." << endl;
                        std::string msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + "set new topic :" + topic + "\r\n";
                        for (it = _canals[searchCanal(channels)].members.begin(); it != _canals[searchCanal(channels)].members.end(); it++)
                            send(_sockets_array[searchClient(*it) + 1].fd, msg.c_str(), msg.size(), 0);
                    }
                    else
                    {
                        cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not operator)." << endl;
                        return (ERR_CHANOPRIVSNEEDED);
                    }
                }
                else{
                    _canals[searchCanal(channels)].topic = topic;
                    cout << getTime() << _clients_data[id].nickname << " changed the topic of " << channels << " to " << topic << "." << endl;
                    std::string msg = ":" + _clients_data[id].nickname + " TOPIC " + channels + "set new topic :" + topic + "\r\n";
                    for (std::vector<std::string>::iterator it = _canals[searchCanal(channels)].members.begin(); it != _canals[searchCanal(channels)].members.end(); it++)
                        send(_sockets_array[searchClient(*it) + 1].fd, msg.c_str(), msg.size(), 0);
                }
            }
            else
            {
                cout << getTime() << "Error! " << _clients_data[id].nickname << " failed to request (not in the channel)." << endl;
                return (ERR_NOTONCHANNEL);
            }
        }
    }
    return (0);
}
