#ifndef QUEUE_H
#define QUEUE_H

#include "PCB.h"


typedef struct QueueNode {
    PCB *pcb; // PCB pointer
    struct QueueNode *next;
} QueueNode;


typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;


Queue *createQueue();
void enqueue(Queue *queue, PCB *pcb);
PCB *dequeue(Queue *queue,int pid);
int isEmpty(Queue *queue);
void destroyQueue(Queue *queue);


#endif
