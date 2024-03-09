#include <stdio.h>
#include <limits.h>
#include "Queue.h"
#include "PCB.h"

PCB *priorityQueue(Queue *queue) {
    if (isEmpty(queue))
        return NULL;
    PCB *selectedPCB = NULL;
    int highestPriority = INT_MAX; 
    QueueNode *currentNode = queue->front;

    while (currentNode != NULL) {
        PCB *currentPCB = currentNode->pcb;
        int currentPriority = currentPCB->priority;
        if (currentPriority < highestPriority) {
            highestPriority = currentPriority;
            selectedPCB = currentPCB;
        }
        currentNode = currentNode->next;
    }
    
    dequeue(queue, selectedPCB->process_id);
    return selectedPCB;
}
