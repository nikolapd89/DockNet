//
//  server.h
//  DockNet
//
//  Created by nikola.stojanovic.ext on 6.12.23..
//

#ifndef server_h
#define server_h

#include <stdio.h>
#include <netdb.h>

#define BACKLOG 10
#define MAXDATASIZE 100
#define PORT "9090"

/// Sets up the server socket and returns its file descriptor
int initialize_server_socket(void);
/// Accepts incoming connections from client
int establish_client_connection(int server_socket);
/// Handles communication with the connected client
void process_client_request(int client_socket);

#endif /* server_h */
