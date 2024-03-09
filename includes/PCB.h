#ifndef PCB_H
#define PCB_H

typedef struct PCB {
    int process_id;
    int priority;
    int num_cpu_bursts;
    int num_io_bursts;
    int total_burst_times;
    int current_cpu_burst; // Track the current CPU burst
    int current_io_burst;  // Track the current I/O burst
    double *cpu_burst_times;
    double *original_cpu_burst_times;
    double *io_burst_times;
    double *waiting_times;
    double arrival_time; // set when entered into the ready queue
} PCB;

// Function prototypes
PCB *createPCB(int process_id, int priority, int num_bursts,int* burst_times);
double calculateWaitingTime(PCB *pcb);
double calculateTurnaroundTime(PCB *pcb);
double calculateBurstTime(PCB *pcb);
int updateCPUState(PCB *pcb, double wait_time);
int updateIOState(PCB *pcb);
int nextCPUburstNumber(PCB *pcb);
int nextIOburstNumber(PCB *pcb);

#endif
