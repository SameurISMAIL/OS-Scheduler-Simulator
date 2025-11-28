# OS-Scheduler-Simulator
**A simple, educational Operating System CPU Scheduling Algorithms Simulator written in C**

GitHub: https://github.com/SameurISMAIL/OS-Scheduler-Simulator  

---

### Overview  
This project implements and compares several classic CPU scheduling algorithms used in operating systems:

- **FCFS** – First Come First Served  
- **SJF** – Shortest Job First (non-preemptive)  
- **SRTF** – Shortest Remaining Time First (preemptive)  
- **Round Robin** – with configurable time quantum  
- **Priority Scheduling** (non-preemptive & preemptive versions)  

The simulator reads a list of processes from a text file, executes each algorithm, and displays detailed results: Gantt chart, waiting time, turnaround time, and average values.

Perfect for Operating Systems courses or anyone wanting to visualize how scheduling decisions affect performance.

---

### Project Structure
```
OS-Scheduler-Simulator/
│
├── algorithms/          ← Folder containing algorithm implementations
├── lire_processus.c     ← Reads processes from params.txt
├── main.c               ← Menu & program entry point
├── process.h            ← Process structure definition
├── params.txt           ← Input file (you can edit it!)
├── Makefile             ← Compile with a single command
└── README.md            ← This file
```

---

### How to Compile & Run (Linux / macOS / WSL)

```bash
# 1. Clone the repository
git clone https://github.com/SameurISMAIL/OS-Scheduler-Simulator.git
cd OS-Scheduler-Simulator

# 2. Compile (uses the provided Makefile)
make

# 3. Run the simulator
./game params.txt 
```

The program displays a menu where you can choose any algorithm and see the full execution trace.

---

### Input File Format – params.txt
Each line represents a process:
```
ProcessID ArrivalTime BurstTime Priority
```

Example (`params.txt` included):
```
P1 0 24 3
P2 3 3 1
P3 6 3 4
P4 8 12 2
```

Feel free to modify or replace `params.txt` with your own test cases.

---

### Sample Output
```
========= Algorithm Game =========
1. RR (Round Robin)
2. SJF (Shortest Job First)
3. FIFO (First In First Out)
4. SRT (Shortest Remaining Time)
5. PRIORITY
6. Exit
==================================
Enter your choice: 1

Welcome to Round Robin Scheduling Algorithm
| PID | Arrival Time | Burst Time | Priority |
|-----|--------------|------------|----------|
| P1  | 0            | 4          | 5        |
| P2  | 1            | 3          | 1        |
| P3  | 2            | 2          | 4        |
| P4  | 0            | 2          | 2        |
| P5  | 0            | 1          | 3        |

Enter Time Slice OR Quantum Number: 2
Gantt chart:
|0-P1-2-P4-4-P5-5-P2-7-P3-9-P1-11-P2-12|
```


### License
This project is open-source and available under the MIT License. Feel free to use, modify, and distribute it for educational or personal purposes.

Enjoy experimenting with scheduling algorithms!  
