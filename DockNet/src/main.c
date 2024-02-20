//
//  main.c
//  DockNet
//
//  Created by nikola.stojanovic.ext on 27.11.23..
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"
#include "client_info.h"

/// Server runloop flag
volatile sig_atomic_t server_running = 1;

/// POSIX signal handler
void signal_handler(int signal);

/* `int argc` represents the count of arguments being passed down to our program,
while `const char * argv[]` is a collection of passed in arguments.
The count includes the program's name itself, so `argc` will always be at least 1 in every running program.
For instance, if we were to run a program like `./server arg1 arg2`, `argc` will be equal to 3. */

int main(int argc, const char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);

    /* Set the `sa_handler` to point to our custom signal handler function.
    This function will be called when the specified signal is received. */

    sa.sa_handler = signal_handler;

    /* `sigaction` sets the action to be taken when the SIGINT signal is received.
    SIGINT is typically sent when the user presses Ctrl+C.
    This allows the program to perform a graceful shutdown, freeing resources before exiting. */

    sigaction(SIGINT, &sa, NULL);

    /* Initialize the server socket by setting up its address, binding it, and setting it to listen.
    `initialize_server_socket` is our custom function that encapsulates these steps. */

    int server_socket = initialize_server_socket();

    /* The main server loop. `server_running` is a flag that should be declared globally and initially set to 1.
    It will be set to 0 by `signal_handler` when SIGINT is received, causing the loop to exit. */

    while (server_running)
    {
        int client_socket = establish_client_connection(server_socket);

        if (client_socket == -1)
            continue;

        print_client_info(client_socket);

        printf("Server got connection\n");

        process_client_request(client_socket);
    }
    return 0;
}

void signal_handler(int signal)
{
    printf("\nServer shutting down...\n");
    server_running = 0;
}
