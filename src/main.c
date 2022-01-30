#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listen.h"
#include "handle_connection.h"
#include "utils.h"

#define SERVER_PORT 8080

response_t *generate_response(request_t *req)
{
  response_t *res = malloc(sizeof(response_t));

  // Display params
  if (strlen(req->params) > 0)
  {
    printf("\nParams: %s\n", req->params);
  }

  // GET / -> 200
  if (strcmp(req->uri, "/") == 0 && strcmp(req->method, "GET") == 0)
  {
    res->status = 200;
    strcpy(res->headers, "Content-Type: text/html\r\n");
    strcpy(res->body, "<html><body><h1>Welcome on the main page!</h1></body></html>");
    return res;
  }

  // (GET/HEAD) /cats -> 200
  if (strcmp(req->uri, "/cats") == 0 && (strcmp(req->method, "GET") == 0 || strcmp(req->method, "HEAD") == 0))
  {
    res->status = 200;
    strcpy(res->headers, "Content-Type: text/html\r\n");
    strcpy(res->body, "<html><body><h1>Cats!</h1></body></html>");
    return res;
  }

  // POST /cats -> 201
  if (strcmp(req->uri, "/cats") == 0 && strcmp(req->method, "POST") == 0)
  {
    res->status = 201;
    printf("\nCat created!\nCat data: %s\n", req->body);
    strcpy(res->headers, "Content-Type: text/html\r\n");
    strcpy(res->body, "<html><body><h1>Cat added!</h1></body></html>");
    return res;
  }

  // * * -> 404
  res->status = 404;
  strcpy(res->headers, "Content-Type: text/html\r\n");
  strcpy(res->body, "<html><body><h1>404 Not Found!</h1></body></html>");
  return res;
}

int main(int argc, char **argv)
{
  server_listen(SERVER_PORT, &generate_response, true);

  return 0;
}
