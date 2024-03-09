#include <stdio.h>
#include <limits.h>
#include "Queue.h"
#include "PCB.h"


PCB *sjf(Queue *queue) {
    if (isEmpty(queue))
        return NULL;
    PCB *selectedPCB = NULL;
    int minCPUBurst = INT_MAX; 
    QueueNode *currentNode = queue->front;
    while (currentNode != NULL) {
        PCB *currentPCB = currentNode->pcb;
        int nextCPUBurst = currentPCB->cpu_burst_times[currentPCB->current_cpu_burst];
        if (nextCPUBurst < minCPUBurst) {
            minCPUBurst = nextCPUBurst;
            selectedPCB = currentPCB;
        }
        currentNode = currentNode->next;
    }

    dequeue(queue, selectedPCB->process_id);

    return selectedPCB;
}
