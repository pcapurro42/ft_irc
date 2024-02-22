#include "../Server.hpp"
#include <ctime>

void sendTime(std::string nickname, int sock){
    std::time_t currentTime = std::time(NULL); 
    std::tm* localTime = std::localtime(&currentTime);

    char buffer[80];
    std::strftime(buffer, 80, "%H:%M:%S", localTime);
    string bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :Time is : " + buffer + " !\r\n";
    send(sock, bot_msg.c_str(), bot_msg.length(), 0);

}

void sendDate(std::string nickname, int sock){
    std::time_t currentTime = std::time(NULL); 
    std::tm* localTime = std::localtime(&currentTime);

    char buffer[80];
    std::strftime(buffer, 80, "%Y:%m:%d", localTime);
    string bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :Date is : " + buffer + " !\r\n";
    send(sock, bot_msg.c_str(), bot_msg.length(), 0);
}

void Server::botTOTD(int id, std::string msg){
    string nickname = _clients_data[id].nickname;
    std::cout << msg << std::endl;

   if (msg == ":!time"){
        sendTime(nickname, _sockets_array[id + 1].fd);
   }
   else if(msg == ":!date"){
        sendDate(nickname, _sockets_array[id + 1].fd);
   }
   else{
    string bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :Sorry friend ! I only answer to !time and !date !\r\n";
    send( _sockets_array[id + 1].fd, bot_msg.c_str(), bot_msg.length(), 0);
   }
}

int Server::executeBotCommand(std::string cmd, int id){
    
    string nickname = _clients_data[id].nickname;
    if (cmd != "BOT"){
        cout << "Error !" << nickname << " typed a command with too many parameters." << endl;
        return (ERR_TOOMANYPARAMS);
    }
    if (_clients_data[id].authentified == false)
    {
        cout << "Error! " << nickname << " failed to request (not authentified)." << endl;
        return (ERR_NOTREGISTERED);
    }
    if (_clients_data[id].identified == false)
    {
        cout << "Error! " << nickname << " failed to request (not identified)." << endl;
        return (ERR_NOPRIVILEGES);
    }


    // std::time_t t = std::time(NULL);
    
    // // Convertir le temps en une structure tm
    // std::tm* now = std::localtime(&t);
    
    // // Afficher la date
    // std::cout << "Date du jour : ";
    // std::cout << (now->tm_year + 1900) << '-' 
    //           << (now->tm_mon + 1) << '-'
    //           <<  now->tm_mday << std::endl;

    string bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :Hello dear " \
                        + nickname + "!" + " Im Gipiti, the time bot !\r\n";
    send(_sockets_array[id + 1].fd, bot_msg.c_str(), bot_msg.length(), 0);
    bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :Feel free to reach me with /BOT everytime you want to check the clock !\r\n" ;
    send(_sockets_array[id + 1].fd, bot_msg.c_str(), bot_msg.length(), 0);
    bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :If you want to get the date, type \"!date\".\r\n";
    send(_sockets_array[id + 1].fd, bot_msg.c_str(), bot_msg.length(), 0);
    bot_msg = ":Gipiti_bot PRIVMSG " + nickname + " :If you want to get the time, type \"!time\".\r\n";
    send(_sockets_array[id + 1].fd, bot_msg.c_str(), bot_msg.length(), 0);
    return (0);

}