#ifndef CONSOLEH
#define CONSOLEH

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define CONSOLE_MAX_LINES    10
#define CONSOLE_MAX_LEN_LINE 100

void read_line(char *buff);
void reset_console();

#endif
