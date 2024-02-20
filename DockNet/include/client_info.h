//
//  client_info.h
//  DockNet
//
//  Created by nikola.stojanovic.ext on 6.12.23..
//

#ifndef client_info_h
#define client_info_h

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>

/// Print client's IP address and port
void print_client_info(int client_socket);

#endif /* client_info_h */
