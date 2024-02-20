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

int main(int argc, const char * argv[]) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, NULL);
    int server_socket = initialize_server_socket();
    while (server_running) {
        int client_socket = establish_client_connection(server_socket);
        if (client_socket == -1) continue;
        print_client_info(client_socket);
        printf("Server got connection\n");
        process_client_request(client_socket);
    }
    return 0;
}

void signal_handler(int signal) {
    printf("\nServer shutting down...\n");
    server_running = 0;
}
