/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcapurro <pcapurro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:31:21 by pcapurro          #+#    #+#             */
/*   Updated: 2024/02/18 21:15:19 by pcapurro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <cstdlib>
# include <sstream>
# include <string>

# include <unistd.h>

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <arpa/inet.h>

# include <fcntl.h>
# include <signal.h>
# include <poll.h>
# include <netdb.h>

# include <algorithm>
# include <vector>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

int     main(const int argc, const char **argv);
void    shutdown(int signal);

void    printTitle(void);
void    printMessage(const char *message, bool end);
int     printError(int value);

#endif