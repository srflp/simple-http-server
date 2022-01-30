#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "handle_connection.h"
#include "status_text.h"

#define BUFFER_SIZE 4096
#define HTTP_VERSION "HTTP/1.1"

void *handle_connection(void *thread_args)
{
  struct ThreadArgs *args = (struct ThreadArgs *)thread_args;
  int client_socket = args->client_socket;
  response_t *(*response_generator)() = args->response_generator;
  free(thread_args);

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  int message_size = 0;
  request_t *req = malloc(sizeof(request_t));
  while ((bytes_read = read(client_socket, buffer + message_size, sizeof(buffer) - message_size - 1)) > 0)
  {
    message_size += bytes_read;
    parse_request(buffer, req);

    if (message_size > BUFFER_SIZE - 1 || buffer[message_size - 1] == '\n' || strcmp(req->method, "POST") == 0)
      break;
  }

  if (!(strcmp(req->method, "GET") == 0 || strcmp(req->method, "HEAD") == 0 || strcmp(req->method, "PUT") == 0 || strcmp(req->method, "DELETE") == 0 || strcmp(req->method, "POST") == 0))
  {
    printf("Only GET, HEAD, PUT, DELETE, POST requests are supported\n");
  }

  handle_error(bytes_read, "Failed to read from client socket");

  // prepare response
  response_t *res = response_generator(req);

  char status_line[128];
  snprintf(status_line, 100, "%s %d %s\r\n", HTTP_VERSION, res->status, get_status_text(res->status));

  size_t content_length = strlen(res->body);
  char content_length_header[64];
  snprintf(content_length_header, 64, "Content-Length: %zu\r\n", content_length);

  char *connection_close_header = "Connection: close\r\n";
  char *terminate_headers = "\r\n";

  int body_size = strcmp(req->method, "HEAD") == 0 ? 0 : strlen(res->body);
  char *response = malloc(strlen(status_line) + strlen(content_length_header) + strlen(connection_close_header) + strlen(res->headers) + strlen(terminate_headers) + body_size + 1);
  strcat(response, status_line);
  strcat(response, content_length_header);
  strcat(response, connection_close_header);
  strcat(response, res->headers);
  strcat(response, terminate_headers);
  if (strcmp(req->method, "HEAD") != 0)
    strcat(response, res->body);

  // send response and close connection
  write(client_socket, response, strlen(response));
  close(client_socket);

  // printing request-response info
  printf("%s %s -> %d %s\n", req->method, req->uri, res->status, get_status_text(res->status));

  free(res);

  return NULL;
}