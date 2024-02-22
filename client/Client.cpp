/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 19:04:29 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/22 23:16:42 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <sstream>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <netdb.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

int c_socket;

void    shutdown(int signal)
{
    close(c_socket);
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Error! Invalid nummber of arguments." << endl;
        cerr << "Usage: ./Client [port]" << endl;
        return (1);
    }

    signal(SIGINT, shutdown);

    string  str;

    c_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct in_addr      ip_s;
    const char          *ip_ad = "127.0.0.1";
    inet_pton(AF_INET, ip_ad, &ip_s);

    struct sockaddr_in  addr_str;
    addr_str.sin_family = AF_INET;
    addr_str.sin_port = htons(atoi(argv[1]));
    addr_str.sin_addr.s_addr = ip_s.s_addr;
    for (int i = 0; i != 8; i++)
        addr_str.sin_zero[i] = 0;
    
    int value = connect(c_socket, ((const sockaddr *)&addr_str), sizeof(addr_str));
    if (value != -1)
        cout << "Connected." << endl;
    else
        cout << "Failed to connect." << endl, shutdown(0);
    while (6)
    {
        std::getline(std::cin, str);
        send(c_socket, (str + "\n").c_str(), str.length() + 2, 0);
    }
    return (0);
}