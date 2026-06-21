# BANKER'S ALGORITHM - DEADLOCK AVOIDANCE SIMULATOR

# WHAT THIS PROGRAM DOES


This program implements the Banker's Algorithm to check if a system is in a **SAFE** or **UNSAFE** state. It prevents deadlocks by simulating resource allocation to processes.


# HOW TO RUN

# Compile:

```bash
g++ bankers_algorithm.cpp -o bankers
```

# SAMPLE OUTPUT

Enter the total number of processes--- 2
Enter the total number of resource types--- 2

Enter total number of instances for resource type: 1: 10
Enter total number of instances for resource type: 2: 8

Enter the max list for all processes:

Max list for process 1:
For resource type 1: 7
For resource type 2: 5

Max list for process 2:
For resource type 1: 6
For resource type 2: 4

Now enter the allocated list for each process:

Enter allocated list for process 1:
resource type 1: 3
resource type 2: 2

Enter allocated list for process 2:
resource type 1: 2
resource type 2: 1

![alt text](<Screenshot 2026-05-07 133707.png>)

# PROGRAM STRUCTURE
<img width="555" height="157" alt="Screenshot 2026-05-11 082922" src="https://github.com/user-attachments/assets/8abe7185-11a5-419c-85de-b5dfd0894c2f" />

# DATA STRUCTURES
Linked Lists:- Store resources and processes dynamically
Global Variables: Maintain system state
Dynamic Arrays: Track finished processes
