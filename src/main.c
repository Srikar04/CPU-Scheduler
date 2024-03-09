#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "Queue.h"
#include "fileReader.h"
#include "cpuScheduler.h"
#include "ioSubSystem.h"
#include "clock.h"

/*
 *  Name : M.srikar
 *  Id number : 21MCME19
 *  mySched algorithm uses round-robin with shortest job first
 *  Assumptions :
 *  1)It is assumed that the next waiting time of a certain process will 
 *  be after removing the previous waiting time from the given wait_time.
 *  2) It is assumed that while calculating turnaround time for round-robin and mySched 
 *  algorithm we can use the traditional approach of calculating 
 *  turnaround_time = original_burst_time + waiting_time
 *  because both these algorithms have multiple cpu burst times for a given burst time
*/

// Global variables for mutex lock, ready queue, and wait queue
pthread_mutex_t mutex;
Queue *ready_queue;
Queue *wait_queue;
Queue* terminate_queue;
volatile bool scheduler_running = true;
volatile bool file_reading = true;
double cpu_util;
char* algoType;
char* fileName;
int timeQuantum;


int main(int argc, char* argv[]) {
    algoType = (char*)malloc(100 * sizeof(char));
    fileName = (char*)malloc(100 * sizeof(char));
    // Extract values from command-line arguments
    strcpy(algoType, argv[2]);
    if(strcmp(algoType,"rr") == 0 || strcmp(algoType,"mySched") == 0){
        timeQuantum = atoi(argv[4]);
        strcpy(fileName, argv[6]);
    }else{
        strcpy(fileName, argv[4]);
    }
    // starting the clock
    initializeStartTime();
    // Initialize the mutex lock
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(1);
    }

    // Create the ready queue and wait queue
    ready_queue = createQueue();
    wait_queue = createQueue();
    terminate_queue = createQueue();

    // Create and start the fileReader thread
    pthread_t fileReaderThreadID;
    pthread_t cpuScheduleThreadID;
    pthread_t ioThreadID;


    if (pthread_create(&fileReaderThreadID, NULL, fileReaderThread, fileName) != 0 ||
        pthread_create(&cpuScheduleThreadID, NULL, cpuScheduleThread , algoType) != 0 ||
        pthread_create(&ioThreadID, NULL, ioThread, algoType) != 0 ){
        perror("Thread creation failed");
        exit(1);
    }

    // Wait for the fileReader thread to finish
    pthread_join(fileReaderThreadID, NULL);
    pthread_join(cpuScheduleThreadID, NULL);
    pthread_join(ioThreadID, NULL);

     
    // Cleanup and destroy the mutex lock
    pthread_mutex_destroy(&mutex);

    // Cleanup and destroy the queues
    destroyQueue(ready_queue);
    destroyQueue(wait_queue);

    double avg_waiting_time = 0;
    double avg_turn_around_time = 0;
    double throughput = 0;
    double sum_arr_time = 0;
    int num_of_proc = 0;

    printf("\nThe Waiting times of Process are:\n");
    while (!isEmpty(terminate_queue)) {
        PCB *pcb = dequeue(terminate_queue,terminate_queue->front->pcb->process_id);
        num_of_proc++;
        printf("Process ID: %d\n", pcb->process_id);
        printf("Priority: %d\n", pcb->priority);
        printf("arrival: %f\n",  pcb->arrival_time);
        printf("Waiting Times: [");
        for (int i = 0; i < pcb->num_cpu_bursts; i++){
            printf("%f", pcb->waiting_times[i]);
            if (i < pcb->num_cpu_bursts - 1) {
                printf(", ");
            }
        }
        printf("]\n");
        avg_waiting_time = calculateWaitingTime(pcb);
        avg_turn_around_time = calculateTurnaroundTime(pcb);
    }


    avg_waiting_time = avg_waiting_time/num_of_proc;
    avg_turn_around_time = avg_turn_around_time/num_of_proc;
    cpu_util = (cpu_util / getElapsedTime()) * 100;
    throughput = (num_of_proc/getElapsedTime());

    printf("\nOutput is:(Note that all values are in seconds)\n\n");
    printf("Input file Name: %s\n",fileName);
    printf("CPU Scheduling Algorithm: %s\n",algoType);
    printf("CPU utilization: %f Percent\n",cpu_util);
    printf("Throughput: %f process per millisecond\n",throughput);
    printf("Avg. Turnaround time: %f s\n",avg_turn_around_time);
    printf("Avg. Waiting time in the R Queue: %f s\n",avg_waiting_time);

    return 0;
}
