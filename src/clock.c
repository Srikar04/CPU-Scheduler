#include "clock.h"
#include <time.h>

static struct timespec start_time;

void initializeStartTime() {
    clock_gettime(CLOCK_REALTIME, &start_time);
}

double getElapsedTime() {
    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);

    // Calculate the elapsed time in seconds
    double elapsed_time = (current_time.tv_sec - start_time.tv_sec) +
                          (current_time.tv_nsec - start_time.tv_nsec) / 1e9;

    return elapsed_time;
}
