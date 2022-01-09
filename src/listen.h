#pragma once
#include <stdbool.h>

void server_listen(int server_port, char *(*response_callback)(), bool use_threads);