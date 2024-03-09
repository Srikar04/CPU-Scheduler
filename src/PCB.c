#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCB.h"

extern char* algoType;
extern int timeQuantum;

PCB *createPCB(int process_id, int priority, int num_bursts,int* burst_times) {
    PCB *pcb = (PCB *)malloc(sizeof(PCB));

    pcb->process_id = process_id;
    pcb->priority = priority;
    pcb->num_cpu_bursts = (num_bursts + 1) / 2; // Calculate the size of CPU burst array
    pcb->num_io_bursts = num_bursts / 2;        // Calculate the size of I/O burst array
    pcb->total_burst_times = num_bursts;
    pcb->current_cpu_burst = 0;
    pcb->current_io_burst = 0;
    // Allocate memory for arrays
    pcb->cpu_burst_times = (double*)malloc(pcb->num_cpu_bursts * sizeof(int));
    pcb->original_cpu_burst_times = (double *)malloc(pcb->num_cpu_bursts * sizeof(int));
    pcb->io_burst_times = (double *)malloc(pcb->num_io_bursts * sizeof(int));
    pcb->waiting_times = (double *)malloc(pcb->num_cpu_bursts* sizeof(int));
    // Initialize arrays
    for (int i = 0; i < num_bursts; i++) {
        if (i % 2 == 0) {
            pcb->cpu_burst_times[i / 2] = burst_times[i];
            pcb->original_cpu_burst_times[i /2] = burst_times[i];
        } else {
            pcb->io_burst_times[i / 2] = burst_times[i];
        }
    }
    pcb->arrival_time = -111;
    return pcb;
}

int nextCPUburstNumber(PCB *pcb){
    if (pcb->current_cpu_burst < pcb->num_cpu_bursts) {
        return pcb->current_cpu_burst;
    } else {
        return -1; // No more CPU bursts to execute
    }
}

int nextIOburstNumber(PCB *pcb){
    if (pcb->current_io_burst < pcb->num_io_bursts) {
        return pcb->current_io_burst;
    } else {
        return -1; // No more io bursts to execute
    }
}

int updateIOState(PCB *pcb){ // returns 0 if success and -1 if failure
    if(strcmp(algoType,"rr") == 0 || strcmp(algoType,"mySched") == 0){
        if(pcb->io_burst_times[pcb->current_io_burst] <= timeQuantum){
            pcb->current_io_burst++;
            return 0;
        }else{
            pcb->io_burst_times[pcb->current_io_burst] -= timeQuantum;
            return -1;
        }
    }else{
        pcb->current_io_burst++;
        return 0;
    }
}



// the updateCPUState return 0 if everything is normal
// it returns 1 if current_cpu_state has not been updated.

int updateCPUState(PCB *pcb, double wait_time) { // returns 0 if normal and -1 if failure
    if(strcmp(algoType,"rr") == 0 || strcmp(algoType,"mySched") == 0){
        if(pcb->cpu_burst_times[pcb->current_cpu_burst] <= timeQuantum){
            if(pcb->current_cpu_burst != 0) // no change requrired proceed normally
                if(pcb->cpu_burst_times[pcb->current_cpu_burst] == pcb->original_cpu_burst_times[pcb->current_cpu_burst])
                    pcb->waiting_times[pcb->current_cpu_burst] = wait_time - pcb->waiting_times[pcb->current_cpu_burst-1];
                else
                    pcb->waiting_times[pcb->current_cpu_burst] = wait_time;
            else
                pcb->waiting_times[pcb->current_cpu_burst] = wait_time;
        pcb->current_cpu_burst++;
        return 0;
        }else if(pcb->current_cpu_burst > timeQuantum){
            if(pcb->waiting_times[pcb->current_cpu_burst] == 0){ // first time
                pcb->waiting_times[pcb->current_cpu_burst] = wait_time - pcb->waiting_times[pcb->current_cpu_burst - 1];
            }else{ // visiting second time
                pcb->waiting_times[pcb->current_cpu_burst] += wait_time - pcb->waiting_times[pcb->current_cpu_burst];
            }
            pcb->cpu_burst_times[pcb->current_cpu_burst] -= timeQuantum;
        return -1;
        }
    }else{
        if(pcb->current_cpu_burst != 0)
                pcb->waiting_times[pcb->current_cpu_burst] = wait_time - pcb->waiting_times[pcb->current_cpu_burst - 1];
        else
            pcb->waiting_times[pcb->current_cpu_burst] = wait_time;
        pcb->current_cpu_burst++;
        return 0;
    }
}


double calculateWaitingTime(PCB *pcb){
    double wait_time = 0;
    for (int i = 0; i < pcb->num_cpu_bursts; i++) {
        wait_time += pcb->waiting_times[i];
    }
    return wait_time;
}

double calculateTurnaroundTime(PCB *pcb){
    double turn_around_time = 0;
    for (int i = 0; i < pcb->num_cpu_bursts; i++) {
        turn_around_time += pcb->waiting_times[i] + (pcb->original_cpu_burst_times[i]/1000);
    }
    return turn_around_time;
}

double calculateBurstTime(PCB *pcb){
    double burst_time = 0;
    for (int i = 0; i < pcb->num_cpu_bursts; i++){
        burst_time += (double)pcb->original_cpu_burst_times[i];
    }
    return burst_time/1000; //because all burst times in milliseconds convert to seconds
}
