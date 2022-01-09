#include <stdio.h>
#include <stdlib.h>
#include "listen.h"

#define SERVER_PORT 8080

int main(int argc, char **argv)
{
  printf("Server started\n");

  server_listen(SERVER_PORT, true);

  return 0;
}
