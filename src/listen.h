#pragma once
#include <stdbool.h>
#include "handle_connection.h"

void server_listen(int server_port, response_t *(*response_callback)(), bool use_threads);