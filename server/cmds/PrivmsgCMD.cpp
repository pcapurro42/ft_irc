/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:46:37 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/19 22:45:46 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"


int Server::executePrivmsgCommand(string cmd, int id)
{
    int space_nb = std::count(cmd.begin(), cmd.end(), ' ');

    if (space_nb > 2)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with too many paramaters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    else if (space_nb < 2)
    {
        cout << "Error! " << _clients_data[id].nickname << " typed a command with not enough paramaters." << endl;
        return (ERR_NEEDMOREPARAMS);
    }

    string target = getArgument(cmd, 1);

    if (target[0] == '#' && searchCanal(target) == -1)
    {
        cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent channel." << endl;
        return (ERR_NOSUCHCHANNEL);
    }
    if (target[0] != '#' && searchClient(target) == -1 && target != "Gipiti_bot")
    {
        cout << "Error! " << _clients_data[id].nickname << " searched for a non-existent user." << endl;
        return (ERR_NOSUCHNICK);
    }
    if (_clients_data[id].authentified == false)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified == false)
    {
        cout << "Error! " << _clients_data[id].nickname << " failed to request (not identified)." << endl;
        return (ERR_NOPRIVILEGES);
    }
    
    target = getArgument(cmd, 1);
    string message = getArgument(cmd, 2);
    string sender_user = _clients_data[id].nickname;

    if (target == "Gipiti_bot"){
        botTOTD(id, message);
        return (0);
    }
    if (message[0] == ':')
        message = message.c_str() + 1;
    if (target[0] == '#')
    {
        int i = searchCanal(target);

        if (std::find(_canals[i].members.begin(), _canals[i].members.end(), sender_user) == _canals[i].members.end())
        {
            cout << "Error! " << sender_user << " failed to send a message to " << target << " (didn't join it)." << endl;
            return (ERR_NOTONCHANNEL);
        }
        
        string msg = ":" + sender_user + " PRIVMSG " + target + " " + message + "\r\n";
        std::vector<string>::iterator j = _canals[i].members.begin();
        while (j != _canals[i].members.end())
        {
            send(_sockets_array[searchClient(*j) + 1].fd, msg.c_str(), msg.size(), 0);
            j++;
        }

        cout << sender_user << " sent a message to " << target << "." << endl;
    }
    else
    {
        int target_id = searchClient(target) + 1;

        string target_user = _clients_data[searchClient(target)].nickname;
        
        string msg = ":" + sender_user + " PRIVMSG " + target_user + " :" + message + "\r\n";

        send(_sockets_array[target_id].fd, msg.c_str(), msg.size(), 0);
        cout << sender_user << " sent a private message to " << target_user << "." << endl;
    }
    return (0);
}
