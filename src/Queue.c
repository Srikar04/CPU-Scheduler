#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "clock.h"


// Linked List implementation of Queue data structure

Queue *createQueue() {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL) {
        exit(1);
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    
    return queue;
}

void enqueue(Queue *queue, PCB *pcb) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        // Handle memory allocation error
        exit(1);
    }
    if(pcb->arrival_time == -111){ // if arrival time is negative process enqueued in the for first time
        pcb->arrival_time = getElapsedTime();
    }
    newNode->pcb = pcb;
    newNode->next = NULL;

    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->size++;
}

PCB *dequeue(Queue *queue, int pid) {
    if (isEmpty(queue)) {
        return NULL;
    }
    QueueNode *currentNode = queue->front;
    QueueNode *prevNode = NULL;

    while (currentNode != NULL) {
        if (currentNode->pcb->process_id == pid) {
            if (prevNode != NULL) {
                prevNode->next = currentNode->next;
                if (currentNode == queue->rear) {
                    queue->rear = prevNode;
                }
            } else {
                queue->front = currentNode->next;
                if (currentNode->next == NULL) {
                    queue->rear = NULL; // Queue is now empty
                }
            }
            queue->size--;
            return currentNode->pcb;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }
    return NULL;
}


// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return (queue->size == 0);
}

void destroyQueue(Queue *queue) {
    free(queue); // Free the queue structure itself
}
