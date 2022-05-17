#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

void logger(const char* tag, const char* message) {
    time_t now;
    time(&now);
    char* str_time = strtok(ctime(&now), "\n");
    printf("%s [%s]: %s\n", str_time, tag, message);
}