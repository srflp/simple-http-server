#include <string.h>
#include "utils.h"

void parse_request(char *req_str, request_t *req)
{
  int i = 0;

  // Parse method
  while (req_str[i] != ' ' && req_str[i] != '\0')
    i++;
  strncpy(req->method, req_str, i);
  req_str = req_str + i + 1;

  // Parse uri
  i = 0;
  while (req_str[i] != ' ' && req_str[i] != '\0' && req_str[i] != '?')
    i++;
  strncpy(req->uri, req_str, i);
  req_str = req_str + i;

  // Parse query params
  i = 0;
  if (req_str[i] == '?')
  {
    req_str++;
    while (req_str[i] != ' ' && req_str[i] != '\0')
      i++;
    strncpy(req->params, req_str, i);
    req_str = req_str + i;
  }
  req_str++;

  // Parse HTTP version
  i = 0;
  while (req_str[i] != '\r' && req_str[i] != '\0')
    i++;
  strncpy(req->version, req_str, i);
  req_str = req_str + i + 2;

  // Parse headers
  i = 0;
  while (!(req_str[i] == '\r' && req_str[i + 1] == '\n' && req_str[i + 2] == '\r' && req_str[i + 3] == '\n') && req_str[i] != '\0')
    i++;
  strncpy(req->headers, req_str, i);
  req_str = req_str + i + 4;

  // Parse body
  i = 0;
  while (req_str[i] != '\0')
    i++;
  strncpy(req->body, req_str, i);
}