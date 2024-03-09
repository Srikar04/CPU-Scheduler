#include "PCB.h"
#include "Queue.h"
#include "clock.h"
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include "fifo.h"
#include "sjf.h"
#include "priorityQueue.h"

extern Queue *ready_queue;
extern Queue *wait_queue;
extern Queue* terminate_queue;
extern volatile bool scheduler_running;
extern volatile bool file_reading;
extern double cpu_util;
extern int timeQuantum;
extern pthread_mutex_t mutex;

void* cpuScheduleThread(void *arg) {
    char *algoType = (char *)arg;
    while (scheduler_running) {
        pthread_mutex_lock(&mutex);
        if(!isEmpty(ready_queue)){
            PCB* pcb;
            if (strcmp(algoType, "fifo") == 0) {
                pcb = fifo(ready_queue); // Use the fifo function
            }
            else if (strcmp(algoType, "sjf") == 0) {
                pcb = sjf(ready_queue); // Use the sjf function
            }
            else if(strcmp(algoType,"pr") == 0){
                pcb = priorityQueue(ready_queue); // Use the priorityQueue function
            }
            if (strcmp(algoType, "rr") == 0) { // uses the selcting strategy as fifo
                pcb = fifo(ready_queue); // Use the fifo function
            }
            else if (strcmp(algoType, "mySched") == 0) { // uses the same selecting strategy as sjf
                pcb = sjf(ready_queue); // Use the sjf function
            }
            if (pcb != NULL) {
                if(updateCPUState(pcb,getElapsedTime() - pcb->arrival_time) == 0){
                    usleep(pcb->cpu_burst_times[pcb->current_cpu_burst - 1] * 1000);
                    cpu_util += (double)(pcb->cpu_burst_times[pcb->current_cpu_burst - 1])/1000;
                }else{ // if cpu burst time crossed time quantum
                    usleep(timeQuantum * 1000);
                    cpu_util += (double)(timeQuantum)/1000;
                }
                int index = nextIOburstNumber(pcb);
                if (index == -1) {
                    enqueue(terminate_queue, pcb);
                    if(isEmpty(ready_queue) && isEmpty(ready_queue) && !file_reading){
                        scheduler_running = false;
                    }
                }
                else{
                    enqueue(wait_queue, pcb);
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}