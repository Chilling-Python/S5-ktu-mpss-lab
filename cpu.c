#include <stdio.h>
#include <limits.h>

// Function prototypes for each scheduling algorithm
void fcfs(int processes[], int n, int bt[]);
void sjf(int processes[], int n, int bt[]);
void round_robin(int processes[], int n, int bt[], int quantum);
void priority_scheduling(int processes[], int n, int bt[], int priority[]);

int main() {
    int choice, n, quantum;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    int processes[n], burst_time[n], priority[n];

    // Input burst times and priority values
    printf("Enter the burst times for the processes:\n");
    for (int i = 0; i < n; i++) {
        printf("Burst time for process %d: ", i + 1);
        scanf("%d", &burst_time[i]);
        processes[i] = i + 1;
    }

    // Input priority values for Priority Scheduling
    if (n > 1) {
        printf("Enter the priority for each process :\n");
        for (int i = 0; i < n; i++) {
            printf("Priority for process %d: ", i + 1);
            scanf("%d", &priority[i]);
        }
    }

    do {
        printf("\nChoose Scheduling Algorithm:\n");
        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. Round Robin\n");
        printf("4. Priority Scheduling\n");
        printf("5. Exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("First-Come-First-Served (FCFS) Scheduling\n");
                fcfs(processes, n, burst_time);
                break;
            case 2:
                printf("Shortest Job First (SJF) Scheduling\n");
                sjf(processes, n, burst_time);
                break;
            case 3:
                printf("Round Robin (RR) Scheduling\n");
                printf("Enter the time quantum for Round Robin: ");
                scanf("%d", &quantum);
                round_robin(processes, n, burst_time, quantum);
                break;
            case 4:
                printf("Priority Scheduling\n");
                priority_scheduling(processes, n, burst_time, priority);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);

    return 0;
}

// FCFS Scheduling
void fcfs(int processes[], int n, int bt[]) {
    int wt[n], tat[n];
    wt[0] = 0;

    for (int i = 1; i < n; i++)
        wt[i] = bt[i - 1] + wt[i - 1];

    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];

    float total_wt = 0, total_tat = 0;
    printf("Process\tBurst Time\tWaiting Time\tTurn-Around Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("%d\t%d\t\t%d\t\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turn-Around Time: %.2f\n", total_tat / n);
}

// SJF Scheduling
void sjf(int processes[], int n, int bt[]) {
    int wt[n], tat[n], pos, temp;
    int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = bt[i];

    // Sorting burst times (and process numbers) based on burst times
    for (int i = 0; i < n - 1; i++) {
        pos = i;
        for (int j = i + 1; j < n; j++) {
            if (rt[j] < rt[pos])
                pos = j;
        }

        temp = rt[i];
        rt[i] = rt[pos];
        rt[pos] = temp;

        temp = processes[i];
        processes[i] = processes[pos];
        processes[pos] = temp;

        temp = bt[i];
        bt[i] = bt[pos];
        bt[pos] = temp;
    }

    wt[0] = 0;
    for (int i = 1; i < n; i++)
        wt[i] = bt[i - 1] + wt[i - 1];

    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];

    float total_wt = 0, total_tat = 0;
    printf("Process\tBurst Time\tWaiting Time\tTurn-Around Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("%d\t%d\t\t%d\t\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turn-Around Time: %.2f\n", total_tat / n);
}

// Round Robin Scheduling
void round_robin(int processes[], int n, int bt[], int quantum) {
    int wt[n], tat[n];
    int rem_bt[n];
    for (int i = 0; i < n; i++)
        rem_bt[i] = bt[i];

    int t = 0;
    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;
                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    wt[i] = t - bt[i];
                    rem_bt[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }

    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];

    float total_wt = 0, total_tat = 0;
    printf("Process\tBurst Time\tWaiting Time\tTurn-Around Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("%d\t%d\t\t%d\t\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turn-Around Time: %.2f\n", total_tat / n);
}

// Priority Scheduling
void priority_scheduling(int processes[], int n, int bt[], int priority[]) {
    int wt[n], tat[n], pos, temp;
    int p[n], b[n];
    for (int i = 0; i < n; i++) {
        p[i] = processes[i];
        b[i] = bt[i];
    }

    // Sorting processes based on priority
    for (int i = 0; i < n - 1; i++) {
        pos = i;
        for (int j = i + 1; j < n; j++) {
            if (priority[j] < priority[pos]) 
                pos = j;
        }

        temp = priority[i];
        priority[i] = priority[pos];
        priority[pos] = temp;

        temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;

        temp = b[i];
        b[i] = b[pos];
        b[pos] = temp;
    }

    wt[0] = 0;
    for (int i = 1; i < n; i++)
        wt[i] = b[i - 1] + wt[i - 1];

    for (int i = 0; i < n; i++)
        tat[i] = b[i] + wt[i];

    float total_wt = 0, total_tat = 0;
    printf("Process\tBurst Time\tPriority\tWaiting Time\tTurn-Around Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", p[i], b[i], priority[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turn-Around Time: %.2f\n", total_tat / n);
}
