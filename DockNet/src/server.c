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
struct addrinfo *get_server_addrinfo(const char *port);
/// Prepares the server to listen for incoming connections by binding it to a port
int prepare_server(struct addrinfo *info);
/// Gracefully shutdown the program
void shutdown_server(int socketfd, struct addrinfo *info, int exit_code);

int initialize_server_socket(void)
{
    struct addrinfo *info = get_server_addrinfo(PORT);
    int server_socket = prepare_server(info);
    freeaddrinfo(info);

    /* Opening your preferred Unix based shell and doing `man listen` will open
    the man pages for the related system call;

    The `man` command is usable for any other system call referenced in this codebase,
    so feel free to take a sneak peek into their intended usage and description. */

    if ((listen(server_socket, BACKLOG)) == -1)
    {

        /* `perror` is a library function which we’ll be using to print an error message
        to the standard error output (`stderr`).

        It is typically used in conjunction with system calls or library functions
        that set the global variable `errno` when an error occurs. */

        perror("listen");
        shutdown_server(server_socket, NULL, 1);
    }
    printf("Server now listening on port %s\n", PORT);
    return server_socket;
}

int establish_client_connection(int server_socket)
{
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;

    /* The `accept` system call is used to accept a connection request from a client.
    It takes the server socket file descriptor, a pointer to a sockaddr structure to hold
    the client's address information, and a pointer to a socklen_t variable that contains
    the size of the sockaddr structure.

    On success, `accept` returns a new socket file descriptor for the established connection,
    allowing for communication with the connected client. On failure, it returns -1 and sets
    `errno` to indicate the error. */

    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1)
        perror("accept");
    return client_socket;
}

void process_client_request(int client_socket)
{
    char buf[MAXDATASIZE];
    long bytes_received;

    /* `recv` is used to receive messages from a socket (in this case, `client_socket`).
    It stores the incoming data into `buf` up to `MAXDATASIZE` bytes. The fourth argument,
    set to 0, specifies no flags are used for the operation.

    On success, `recv` returns the number of bytes received and stored in `buf`.
    On failure, it returns -1, setting `errno` to indicate the error. */

    if ((bytes_received = recv(client_socket, buf, MAXDATASIZE, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }

    // Null-terminating the received string for proper handling

    buf[bytes_received] = '\0';

    printf("Server received %s\n", buf);

    close(client_socket);
}

struct addrinfo *get_server_addrinfo(const char *port)
{
    struct addrinfo hints, *info;

    /* Throughout this project we’ll be leveraging the `memset` function so as to initialize memory.
    In C, there’s an important distinction between *allocating* and *initializing* memory.

    This is something which we don’t normally need to care about in higher level languages,
    where such details are abstracted away from us.
    In this scenario, we are directly interacting with memory at a lower level,
    which is a common practice in C programming.

    Memory allocation in C, through functions like `malloc`, simply reserves a block of memory,
    but it doesn't automatically initialize it.

    This means the allocated memory could contain leftover data, essentially garbage values,
    which can lead to undefined behavior if not handled properly.
    That's where the `memset` function comes into play.

    `memset` is used to _initialize_ a block of memory with a specific value.
    By using it we can set all bytes in a block of memory to zero or any other desired value.

    This step ensures that our memory starts in a known state,
    eliminating the risk of unexpected data affecting our program's behavior. */

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;     // Unspecified address family allows for IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // Use my IP

    /* `getaddrinfo` is a modern way to prepare socket address structures for subsequent use
    in socket creation, binding, and other operations.

    It replaces older mechanisms like manually filling out `sockaddr_in` structures for IPv4
    or `sockaddr_in6` for IPv6. By providing `NULL` as the first argument and the desired `port`,
    `getaddrinfo` will prepare address structures suitable for a server listening on `port`.

    The function returns 0 on success and sets `info` to point to a linked list of one or more
    `addrinfo` structures containing response information. */

    int status = getaddrinfo(NULL, port, &hints, &info);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(status));
        exit(1);
    }
    return info;
}

int prepare_server(struct addrinfo *info)
{
    int server_socket = 0;
    struct addrinfo *info_ptr;

    /* Iterate through the linked list of addrinfo structures pointed to by `info`.
    Each node potentially represents a viable option for creating and binding a socket. */

    for (info_ptr = info; info_ptr != NULL; info_ptr = info_ptr->ai_next)
    {
        /* Attempt to create a socket with the specifications provided by the current `addrinfo` structure.
        The `socket` function returns a socket descriptor, or -1 if the socket creation fails. */

        server_socket = socket(info_ptr->ai_family, info_ptr->ai_socktype, info_ptr->ai_protocol);
        if (server_socket == -1)
        {
            perror("socket");
            continue;
        }

        int yes = 1;

        /* To allow the server to rebind to the same port without waiting for the default timeout,
        the `SO_REUSEADDR` socket option is set. This is especially useful for development and testing. */

        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        {
            perror("setsockopt");
            close(server_socket);
            continue;
        }

        /* Attempt to bind the created socket to the address and port specified in the current `addrinfo` structure.
        If binding fails, an error message is printed, the socket is closed, and the loop continues to the next structure. */

        if (bind(server_socket, info_ptr->ai_addr, info_ptr->ai_addrlen) == -1)
        {
            perror("bind");
            close(server_socket);
            continue;
        }
        break;
    }
    if (info_ptr == NULL)
        shutdown_server(server_socket, info, 2);
    return server_socket;
}

void shutdown_server(int socketfd, struct addrinfo *info, int exit_code)
{
    if (socketfd)
        close(socketfd);
    if (info)
        freeaddrinfo(info);
    exit(exit_code);
}
