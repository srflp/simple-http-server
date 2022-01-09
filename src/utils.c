#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void handle_error(int error_code, char *error_message)
{
  if (error_code < 0)
  {
    perror(error_message);
    exit(1);
  }
}