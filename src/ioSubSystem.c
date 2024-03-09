#include "Queue.h"
#include "clock.h"
#include "PCB.h"
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
extern int timeQuantum;
extern pthread_mutex_t mutex;


void* ioThread(void *arg) {
    char *algoType = (char *)arg;
    while (scheduler_running) {
        pthread_mutex_lock(&mutex);
        if(!isEmpty(wait_queue)){
            PCB* pcb;
            if (strcmp(algoType, "fifo") == 0) {
                pcb = fifo(wait_queue); // Use the fifo function
            }
            else if (strcmp(algoType, "sjf") == 0) {
                pcb = sjf(wait_queue); // Use the sjf function
            }
            else if(strcmp(algoType,"pr") == 0){
                pcb = priorityQueue(wait_queue); // Use the priorityQueue function
            }
            if (strcmp(algoType, "rr") == 0) { // uses the selcting strategy as fifo
                pcb = fifo(wait_queue); // Use the fifo function
            }
            else if (strcmp(algoType, "mySched") == 0) { // uses the same selecting strategy as sjf
                pcb = sjf(wait_queue); // Use the sjf function
            }
            if (pcb != NULL) {
                if(updateIOState(pcb) == 0){
                    usleep(pcb->io_burst_times[pcb->current_io_burst - 1] * 1000);
                }else{
                    usleep(timeQuantum * 1000);
                }
                int index = nextCPUburstNumber(pcb);
                if (index == -1) {
                    enqueue(terminate_queue, pcb);
                    if(isEmpty(ready_queue) && isEmpty(wait_queue) && !file_reading){
                        scheduler_running = false;
                    }
                }
                else{
                    enqueue(ready_queue, pcb);
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
