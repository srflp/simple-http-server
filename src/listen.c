#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdbool.h>
#include <pthread.h>
#include "utils.h"
#include "handle_connection.h"

#define SERVER_BACKLOG 100

void server_listen(int server_port, bool use_threads)
{
  // Create socket
  int server_socket;
  handle_error(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket");

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(server_port);

  // Bind socket to the address
  handle_error(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)), "Failed to bind socket");
  // Listen for connections
  handle_error(listen(server_socket, SERVER_BACKLOG), "Failed to listen on socket");

  while (true)
  {
    printf("Waiting for connection\n");

    socklen_t address_size = sizeof(server_address);
    int client_socket;
    handle_error(client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_size), "Failed to accept connection");
    printf("Connection accepted\n");

    // Create a pointer for the client socket (pthreads require pointers)
    int *pclient_socket = malloc(sizeof(int));
    *pclient_socket = client_socket;

    if (use_threads)
    {
      pthread_t t;
      pthread_create(&t, NULL, handle_connection, pclient_socket);
    }
    else
    {
      handle_connection(pclient_socket);
    }
  }
}