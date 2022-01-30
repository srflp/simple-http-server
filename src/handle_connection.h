#pragma once

struct ThreadArgs
{
  int client_socket;
  response_t *(*response_generator)();
};

void parse_request(char *req_str, request_t *req_info);

void *handle_connection(void *thread_args);
