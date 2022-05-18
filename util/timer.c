
#include "timer.h"

int is_time(clock_t time){
    clock_t current_time = clock();
    return current_time > time ? 1 : 0;
}