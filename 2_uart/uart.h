#pragma once

#include "stdint.h"



void uart_init();

void uart_send(char data);

char uart_read();