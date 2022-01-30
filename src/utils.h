#pragma once

void handle_error(int error_code, char *error_message);

typedef struct Request
{
  char method[8];
  char uri[2048];
  char params[2048];
  char version[16];
  char headers[8128];
  char body[8128];
} request_t;

typedef struct Response
{
  int status;
  char headers[8128];
  char body[8128];
} response_t;