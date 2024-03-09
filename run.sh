#!/bin/bash

# Check if the correct number of arguments is provided

# Example : /run.sh Assign3 -algo rr -quant 20 -inp input.txt

if [ $# -lt 2 ]; then
    echo "Usage: Assign3 <command> [args...]"
    exit 1
fi


# Extract the main.c file and command arguments
name="$1"
shift
command_args=("$@")

# Compile the program
# gcc -o  myProg clock.c cpuScheduler.c fileReader.c ioSubSystem.c main.c PCB.c Queue.c fifo.c sjf.c priorityQueue.c -I./includes -lrt

gcc -o myProg src/*.c -I./src -I./includes -lrt

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running your_program..."
    ./myProg "${command_args[@]}"
else
    echo "Compilation failed."
fi
