//
//  client_info.c
//  DockNet
//
//  Created by nikola.stojanovic.ext on 6.12.23..
//

#include "client_info.h"
#include <stdio.h>
#include <arpa/inet.h>

void print_client_info(int client_socket) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;
    int peer = getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (peer == -1) perror("getpeername");
    char client_ip[INET6_ADDRSTRLEN];
    if (client_addr.ss_family == AF_INET) {
        /* IPv4 */
        struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
        inet_ntop(AF_INET, &s->sin_addr, client_ip, sizeof(client_ip));
        printf("Client IP: %s, Port: %d\n", client_ip, ntohs(s->sin_port));
    } else {
        /* IPv6 */
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&client_addr;
        inet_ntop(AF_INET6, &s->sin6_addr, client_ip, sizeof(client_ip));
        printf("Client IP: %s, Port: %d\n", client_ip, ntohs(s->sin6_port));
    }
}


