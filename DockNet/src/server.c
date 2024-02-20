//
//  server.c
//  DockNet
//
//  Created by nikola.stojanovic.ext on 6.12.23..
//

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

/// Gets server address info for a specified port number
struct addrinfo* get_server_addrinfo(const char* port);
/// Prepares the server to listen for incoming connections by binding it to a port
int prepare_server(struct addrinfo *info);
/// Gracefully shutdown the program
void shutdown_server(int socketfd, struct addrinfo *info, int exit_code);

int initialize_server_socket(void) {
    struct addrinfo* info = get_server_addrinfo(PORT);
    int server_socket = prepare_server(info);
    freeaddrinfo(info);
    if ((listen(server_socket, BACKLOG)) == -1) {
        perror("listen");
        shutdown_server(server_socket, NULL, 1);
    }
    printf("Server now listening on port %s\n", PORT);
    return server_socket;
}

int establish_client_connection(int server_socket) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1) perror("accept");
    return client_socket;
}

void process_client_request(int client_socket) {
    char buf[MAXDATASIZE];
    long bytes_received;
    if ((bytes_received = recv(client_socket, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[bytes_received] = '\0';
    printf("Server received %s\n", buf);
    close(client_socket);
}

struct addrinfo* get_server_addrinfo(const char* port) {
    struct addrinfo hints, *info;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Unspecified address family allows for IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // Use my IP
    int status = getaddrinfo(NULL, port, &hints, &info);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(status));
        exit(1);
    }
    return info;
}

int prepare_server(struct addrinfo *info) {
    int server_socket = 0;
    struct addrinfo *info_ptr;
    for (info_ptr = info; info_ptr != NULL; info_ptr = info_ptr->ai_next) {
        server_socket = socket(info_ptr->ai_family, info_ptr->ai_socktype, info_ptr->ai_protocol);
        if (server_socket == -1) {
            perror("socket");
            continue;
        }
        int yes = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            perror("setsockopt");
            close(server_socket);
            continue;
        }
        if (bind(server_socket, info_ptr->ai_addr, info_ptr->ai_addrlen) == -1) {
            perror("bind");
            close(server_socket);
            continue;
        }
        break; // Jackpot
    }
    if (info_ptr == NULL) shutdown_server(server_socket, info, 2);
    return server_socket;
}

void shutdown_server(int socketfd, struct addrinfo *info, int exit_code) {
    if (socketfd) close(socketfd);
    if (info) freeaddrinfo(info);
    exit(exit_code);
}
