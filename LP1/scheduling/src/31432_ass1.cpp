#include <iostream>
using namespace std;

struct Process {
    int id;
    int arrival;
    int burst;
    int priority;
    int completion;
    int waiting;
    int turnaround;
};

// FCFS Scheduling
void fcfs(Process proc[], int n) {
    // Sort by arrival time
    for (int i = 0; i < n -1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (proc[j].arrival > proc[j+1].arrival) {
                Process temp = proc[j];
                proc[j] = proc[j+1];
                proc[j+1] = temp;
            }
        }
    }

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < proc[i].arrival)
            currentTime = proc[i].arrival;
        proc[i].completion = currentTime + proc[i].burst;
        proc[i].turnaround = proc[i].completion - proc[i].arrival;
        proc[i].waiting = proc[i].turnaround - proc[i].burst;
        currentTime = proc[i].completion;
    }

    cout << "\nFCFS Scheduling:\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    int totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << proc[i].id << "\t" << proc[i].arrival << "\t" << proc[i].burst << "\t"
             << proc[i].completion << "\t\t" << proc[i].turnaround << "\t\t" << proc[i].waiting << "\n";
        totalWT += proc[i].waiting;
        totalTAT += proc[i].turnaround;
    }
    cout << "Average Waiting Time = " << (float)totalWT / n << "\n";
    cout << "Average Turnaround Time = " << (float)totalTAT / n << "\n";
}

// Round Robin Scheduling
void roundRobin(Process proc[], int n, int timeQuantum) {
    int completed = 0, currentTime = 0;
    int remainingBurst[100];
    bool inQueue[100] = {false};
    int queue[100];
    int front = 0, rear = 0;

    for (int i = 0; i < n; i++) remainingBurst[i] = proc[i].burst;

    // Enqueue processes which have arrived at time 0
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival <= currentTime) {
            queue[rear++] = i;
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (front == rear) {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && proc[i].arrival <= currentTime && remainingBurst[i] > 0) {
                    queue[rear++] = i;
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int idx = queue[front++];
        int execTime = (remainingBurst[idx] < timeQuantum) ? remainingBurst[idx] : timeQuantum;

        remainingBurst[idx] -= execTime;
        currentTime += execTime;

        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && proc[i].arrival <= currentTime && remainingBurst[i] > 0) {
                queue[rear++] = i;
                inQueue[i] = true;
            }
        }

        if (remainingBurst[idx] == 0) {
            completed++;
            proc[idx].completion = currentTime;
            proc[idx].turnaround = proc[idx].completion - proc[idx].arrival;
            proc[idx].waiting = proc[idx].turnaround - proc[idx].burst;
        } else {
            queue[rear++] = idx;
        }
    }

    cout << "\nRound Robin Scheduling (Time Quantum = " << timeQuantum << "):\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    int totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << proc[i].id << "\t" << proc[i].arrival << "\t" << proc[i].burst << "\t"
             << proc[i].completion << "\t\t" << proc[i].turnaround << "\t\t" << proc[i].waiting << "\n";
        totalWT += proc[i].waiting;
        totalTAT += proc[i].turnaround;
    }
    cout << "Average Waiting Time = " << (float)totalWT / n << "\n";
    cout << "Average Turnaround Time = " << (float)totalTAT / n << "\n";
}

// Priority Scheduling (Non-preemptive)
void priorityScheduling(Process proc[], int n) {
    bool completed[100] = {false};
    int completedCount = 0, currentTime = 0;

    while (completedCount < n) {
        int idx = -1;
        int highestPriority = 1e9;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && proc[i].arrival <= currentTime) {
                if (proc[i].priority < highestPriority) {
                    highestPriority = proc[i].priority;
                    idx = i;
                }
                else if (proc[i].priority == highestPriority && proc[i].arrival < proc[idx].arrival) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        currentTime = max(currentTime, proc[idx].arrival) + proc[idx].burst;
        proc[idx].completion = currentTime;
        proc[idx].turnaround = proc[idx].completion - proc[idx].arrival;
        proc[idx].waiting = proc[idx].turnaround - proc[idx].burst;

        completed[idx] = true;
        completedCount++;
    }

    cout << "\nPriority Scheduling (Non-preemptive):\nProcess\tArrival\tBurst\tPriority\tCompletion\tTurnaround\tWaiting\n";
    int totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << proc[i].id << "\t" << proc[i].arrival << "\t" << proc[i].burst << "\t"
             << proc[i].priority << "\t\t" << proc[i].completion << "\t\t" << proc[i].turnaround << "\t\t" << proc[i].waiting << "\n";
        totalWT += proc[i].waiting;
        totalTAT += proc[i].turnaround;
    }
    cout << "Average Waiting Time = " << (float)totalWT / n << "\n";
    cout << "Average Turnaround Time = " << (float)totalTAT / n << "\n";
}

// SJF Scheduling (Non-preemptive)
void sjfScheduling(Process proc[], int n) {
    bool completed[100] = {false};
    int completedCount = 0, currentTime = 0;

    while (completedCount < n) {
        int idx = -1;
        int shortestBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && proc[i].arrival <= currentTime) {
                if (proc[i].burst < shortestBurst) {
                    shortestBurst = proc[i].burst;
                    idx = i;
                }
                else if (proc[i].burst == shortestBurst && proc[i].arrival < proc[idx].arrival) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        currentTime = max(currentTime, proc[idx].arrival) + proc[idx].burst;
        proc[idx].completion = currentTime;
        proc[idx].turnaround = proc[idx].completion - proc[idx].arrival;
        proc[idx].waiting = proc[idx].turnaround - proc[idx].burst;

        completed[idx] = true;
        completedCount++;
    }

    cout << "\nSJF Scheduling (Non-preemptive):\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    int totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << proc[i].id << "\t" << proc[i].arrival << "\t" << proc[i].burst << "\t"
             << proc[i].completion << "\t\t" << proc[i].turnaround << "\t\t" << proc[i].waiting << "\n";
        totalWT += proc[i].waiting;
        totalTAT += proc[i].turnaround;
    }
    cout << "Average Waiting Time = " << (float)totalWT / n << "\n";
    cout << "Average Turnaround Time = " << (float)totalTAT / n << "\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    Process procFCFS[100], procRR[100], procPriority[100], procSJF[100];

    for (int i = 0; i < n; i++) {
        int at, bt, pr;
        cout << "Enter arrival time for process " << i+1 << ": ";
        cin >> at;
        cout << "Enter burst time for process " << i+1 << ": ";
        cin >> bt;
        cout << "Enter priority for process " << i+1 << " (lower number = higher priority): ";
        cin >> pr;

        procFCFS[i] = {i+1, at, bt, pr};
        procRR[i] = procFCFS[i];
        procPriority[i] = procFCFS[i];
        procSJF[i] = procFCFS[i];
    }

    int timeQuantum;
    cout << "Enter time quantum for Round Robin: ";
    cin >> timeQuantum;

    fcfs(procFCFS, n);
    roundRobin(procRR, n, timeQuantum);
    priorityScheduling(procPriority, n);
    sjfScheduling(procSJF, n);

    return 0;
}

