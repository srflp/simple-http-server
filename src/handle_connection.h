#pragma once

struct ThreadArgs
{
  int client_socket;
  char *(*response_generator)();
};

void *handle_connection(void *thread_args);
