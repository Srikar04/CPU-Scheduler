// fileReader.h (Header file)

#ifndef FILEREADER_H
#define FILEREADER_H

#include "Queue.h" // Include the Queue header

// Function to read input from the input.txt file and populate the ready queue
void *fileReaderThread(void *arg);

#endif
