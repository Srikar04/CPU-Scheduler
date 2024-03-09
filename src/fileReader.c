#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "Queue.h" 

extern pthread_mutex_t mutex;
extern Queue *ready_queue;
extern volatile bool file_reading;

void *fileReaderThread(void *arg) {
     char *filename = (char *)arg;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening input.txt");
        exit(1);
    }

    char line[1024];

    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok((char *)line, ",");

        if (strcmp(token, "proc") == 0) {
            int process_id, priority, num_bursts;
            int* burst_times;
            token = strtok(NULL, ",");
            process_id = atoi(token);
            token = strtok(NULL, ",");
            priority = atoi(token);
            token = strtok(NULL, ",");
            num_bursts = atoi(token);
            token = strtok(NULL, ",");
            burst_times = (int*)malloc(num_bursts * sizeof(int));
            for(int i = 0;i<num_bursts;i++) {
                burst_times[i] = atoi(token);
                token = strtok(NULL, ",");
            }
            // Create and enqueue the PCB
            pthread_mutex_lock(&mutex);
            PCB *pcb = createPCB(process_id, priority, num_bursts, burst_times);
            enqueue(ready_queue, pcb);
            pthread_mutex_unlock(&mutex);
        } else if (strcmp(token, "sleep") == 0) {
            token = strtok(NULL, ",");
            int sleep_time = atoi(token);
            usleep(sleep_time * 1000);
        } else if (strcmp(token, "stop") == 0) {
            file_reading = false;
            break;
        }
    }

    // Close the file
    fclose(file);

    return NULL;
}
