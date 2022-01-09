#include <stdio.h>
#include "listen.h"

#define SERVER_PORT 8080

char *generate_response()
{
  return "Hello World! :)\n";
}

int main(int argc, char **argv)
{
  server_listen(SERVER_PORT, &generate_response, true);

  return 0;
}
