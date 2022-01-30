#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdbool.h>
#include <pthread.h>
#include "utils.h"
#include "handle_connection.h"

#define SERVER_BACKLOG 100

void server_listen(int server_port, response_t *(*response_generator)(), bool use_threads)
{
  // Create socket
  int server_socket;
  handle_error(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket");
  handle_error(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){true}, sizeof(int)), "Failed to set socket options");

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(server_port);

  // Bind socket to the address
  handle_error(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)), "Failed to bind socket");
  // Listen for connections
  handle_error(listen(server_socket, SERVER_BACKLOG), "Failed to listen on socket");
  printf("Listening on port %d\n", server_port);

  while (true)
  {
    // Accept connection
    socklen_t address_size = sizeof(server_address);
    int client_socket;
    handle_error(client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_size), "Failed to accept connection");

    // Pass args to threads
    struct ThreadArgs *thread_args = malloc(sizeof(struct ThreadArgs));
    thread_args->client_socket = client_socket;
    thread_args->response_generator = response_generator;

    if (use_threads)
    {
      pthread_t t;
      pthread_create(&t, NULL, handle_connection, thread_args);
    }
    else
    {
      handle_connection(thread_args);
    }
  }
}