#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "utils.h"

#define BUFFER_SIZE 4096

void *handle_connection(void *pclient_socket)
{
  int client_socket = *((int *)pclient_socket);
  free(pclient_socket);

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  int message_size = 0;
  while ((bytes_read = read(client_socket, buffer + message_size, sizeof(buffer) - message_size - 1)) > 0)
  {
    printf("Received %ld bytes\n", bytes_read);
    message_size += bytes_read;
    printf("Message: %s\n", buffer);
    if (message_size > BUFFER_SIZE - 1 || buffer[message_size - 1] == '\n')
      break;
  }
  handle_error(bytes_read, "Failed to read from client socket");

  sleep(1);
  char *message[5] = {
      "HTTP/1.1 200 OK\n",
      "Content-Length: 12\n",
      "Content-Type: text/plain; charset=utf-8\n",
      "\n",
      "Hello World!"};
  for (int i = 0; i < 5; i++)
  {
    write(client_socket, message[i], strlen(message[i]));
  }

  close(client_socket);
  printf("Connection closed\n");

  return NULL;
}