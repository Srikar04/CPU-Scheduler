# Project Overview

This project implements a CPU scheduler algorithm, with execution facilitated through the provided `run.sh` script. The scheduler operates using a round-robin (RR) algorithm with a specified time quantum.

## Project Structure

- **run.sh:** Shell script to execute the commands.
- **src:** Folder containing all `.c` files.
- **includes:** Folder containing all `.h` files.

## How to Run

### Step 1: Give Execution Permission

```bash
chmod +x run.sh
```
### Step 2: Execute the Program
```
./run.sh Assign3 -algo rr -quant 20 -inp input.txt
```
Replace Assign3 with the appropriate program name, and adjust the algorithm (-algo), time quantum (-quant), and input file (-inp) parameters as needed.

Feel free to customize the script and algorithm parameters according to your specific requirements.


