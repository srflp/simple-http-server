#include <stdio.h>
#include "listen.h"

#define SERVER_PORT 8080

int main(int argc, char **argv)
{
  server_listen(SERVER_PORT, true);

  return 0;
}
