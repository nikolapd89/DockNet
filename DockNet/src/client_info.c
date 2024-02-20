//
//  client_info.c
//  DockNet
//
//  Created by nikola.stojanovic.ext on 6.12.23..
//

#include "client_info.h"
#include <stdio.h>
#include <arpa/inet.h>

void print_client_info(int client_socket)
{
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;

    /* `getpeername` retrieves the remote (peer) address of the specified socket.
    This is useful for obtaining information about the client connected to the server. */

    int peer = getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_size);

    if (peer == -1)
        perror("getpeername");
    char client_ip[INET6_ADDRSTRLEN];

    /* The `sockaddr_storage` structure is large enough to hold either IPv4 or IPv6 addresses.
    We check the `ss_family` field to determine the type of address and cast the structure accordingly. */

    if (client_addr.ss_family == AF_INET)
    {

        /* IPv4 */

        struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;

        /* `inet_ntop` converts the numeric (binary) IP address to a readable string format.
        `AF_INET` specifies that we're dealing with an IPv4 address. */

        inet_ntop(AF_INET, &s->sin_addr, client_ip, sizeof(client_ip));

        /* Print the client's IP address and port number. `ntohs` converts the port number from
        network byte order to host byte order for readability. */

        printf("Client IP: %s, Port: %d\n", client_ip, ntohs(s->sin_port));
    }
    else
    {

        /* IPv6 */

        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&client_addr;

        /* Use `inet_ntop` with `AF_INET6` to handle the IPv6 address. */

        inet_ntop(AF_INET6, &s->sin6_addr, client_ip, sizeof(client_ip));

        /* Print the IPv6 address and port, converting the port number as before. */

        printf("Client IP: %s, Port: %d\n", client_ip, ntohs(s->sin6_port));
    }
}
