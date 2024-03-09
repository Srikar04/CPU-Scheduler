#include "Queue.h"
#include <stdio.h>
#include "PCB.h"

PCB* fifo(Queue *queue) { // fifo never recieves empty queue
    if(isEmpty(queue))
        return NULL;
    PCB *selectedPCB = dequeue(queue,queue->front->pcb->process_id);
    return selectedPCB;
}
