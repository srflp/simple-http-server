#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "utils.h"
#include "handle_connection.h"

#define BUFFER_SIZE 4096

void *handle_connection(void *thread_args)
{
  struct ThreadArgs *args = (struct ThreadArgs *)thread_args;
  int client_socket = args->client_socket;
  char *(*response_generator)() = args->response_generator;
  free(thread_args);

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  int message_size = 0;
  while ((bytes_read = read(client_socket, buffer + message_size, sizeof(buffer) - message_size - 1)) > 0)
  {
    message_size += bytes_read;
    if (message_size > BUFFER_SIZE - 1 || buffer[message_size - 1] == '\n')
      break;
  }

  printf("%s", buffer);

  // TODO Parse request
  // - method
  // - URI
  // - protocol
  // - headers
  // if method is POST:
  // - body

  handle_error(bytes_read, "Failed to read from client socket");

  // sleep(1);

  // Prepare and send response
  char *body = response_generator(); // pass request URI as argument here
  char *header1 = "HTTP/1.1 200 OK\n"
                  "Content-Length: ";

  size_t content_length = strlen(body);
  char content_length_str[12];
  sprintf(content_length_str, "%zu", content_length);

  char *header2 = "\n"
                  "Content-Type: text/plain; charset=utf-8\n"
                  "\n";

  char *response = malloc(strlen(header1) + strlen(content_length_str) + strlen(header2) + strlen(body) + 1);
  strcat(response, header1);
  strcat(response, content_length_str);
  strcat(response, header2);
  strcat(response, body);

  printf("%s", response);

  write(client_socket, response, strlen(response));

  close(client_socket);

  return NULL;
}