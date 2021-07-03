# CS421-CPU-scheduling-algorithms-simulation

This is a C++ program that simulate 3 CPU scheduling algorithms: Round Robin, Shortest job first, and Shortest remaining job first.
The input is the given .txt file "Lab3inputfileF20.txt"
The output will be saved to another .txt file called "output.txt"

### Sample input and output
Input:
A 1 0
B 4 0
C 3 2
D 1 2
E 12 3
F 1 4
G 3 5
H 2 5

Output:
Round Robin Scheduling
0 A    Process terminated
1 B    Process terminated
5 C    Process terminated
8 D    Process terminated
9 E    Process terminated
13 F    Quantum expired
14 G    Process terminated
17 H    Process terminated
19 E    Process terminated
23 E    Quantum expired
27 complete

PrcessID  Turnaround Time  Waiting Time
    A           1                0
    B           5                1
    C           6                3
    D           7                6
    E           24                12
    F           10                9
    G           12                9
    H           14                12
Average    79/8=9.875        52/8=6.5
==================================================

Shortest Job First Scheduling
0 A    Process terminated
1 B    Process terminated
5 D    Process terminated
6 F    Process terminated
7 H    Process terminated
9 C    Process terminated
12 G    Process terminated
15 E    Process terminated
27 complete

PrcessID  Turnaround Time  Waiting Time
    A           1                0
    B           5                1
    C           10                7
    D           4                3
    E           24                12
    F           3                2
    G           10                7
    H           4                2
Average    61/8=7.625        34/8=4.25
==================================================

Shortest Remaining Job First Scheduling
0 A    Process terminated
1 B    Process terminated
2 D    Process preempted by process with shorter burst time
3 B    Process terminated
4 F    Process preempted by process with shorter burst time
5 B    Process terminated
7 H    Process terminated
9 C    Process terminated
12 G    Process terminated
15 E    Process terminated
27 complete

PrcessID  Turnaround Time  Waiting Time
    A           1                0
    B           7                3
    C           10                7
    D           1                0
    E           24                12
    F           1                0
    G           10                7
    H           4                2
Average    58/8=7.25        31/8=3.875
