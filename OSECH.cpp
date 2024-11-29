#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Process {
    int pid;             // Process ID
    int arrival_time;    // Arrival Time
    int burst_time;      // Burst Time
    int completion_time; // Completion Time
    int turnaround_time; // Turnaround Time
    int waiting_time;    // Waiting Time
    int priority;        // Priority
};

void findWaitingTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }
}

void findTurnaroundTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
    }
}

void findCompletionTimeFCFS(Process proc[], int n) {
    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;

    for (int i = 1; i < n; i++) {
        proc[i].completion_time = max(proc[i - 1].completion_time, proc[i].arrival_time) + proc[i].burst_time;
    }
}

void findFCFS(Process proc[], int n) {
    findCompletionTimeFCFS(proc, n);
    findTurnaroundTime(proc, n);
    findWaitingTime(proc, n);
}

void findSJF(Process proc[], int n) {
    int remainingTime[n];
    for (int i = 0; i < n; i++) {
        remainingTime[i] = proc[i].burst_time;
    }

    int complete = 0, currentTime = 0, shortest = -1, minBurst = 9999;

    while (complete < n) {
        minBurst = 9999;
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= currentTime && remainingTime[i] < minBurst && remainingTime[i] > 0) {
                minBurst = remainingTime[i];
                shortest = i;
            }
        }

        remainingTime[shortest]--;
        if (remainingTime[shortest] == 0) {
            complete++;
            proc[shortest].completion_time = currentTime + 1;
            proc[shortest].turnaround_time = proc[shortest].completion_time - proc[shortest].arrival_time;
            proc[shortest].waiting_time = proc[shortest].turnaround_time - proc[shortest].burst_time;
        }
        currentTime++;
    }
}

void FindRR(Process processes[], int n, int quantum) {
  int remainingTime[n];
  for (int i = 0; i < n; i++) {
    remainingTime[i] = processes[i].burst_time;
  }
  int currentTime = 0;
  bool allDone = false;
  while (!allDone) {
    allDone = true;
    for (int i = 0; i < n; i++) {
      if (remainingTime[i] > 0) {
        allDone = false;
        if (remainingTime[i] > quantum) {
          currentTime = currentTime + quantum;
          remainingTime[i] = remainingTime[i] - quantum;
        } else {
          currentTime = currentTime + remainingTime[i];
          processes[i].completion_time = currentTime;
          remainingTime[i] = 0;
        }
      }
    }
  }
}


void findPriorityPreemptive(Process proc[], int n) {
    vector<int> remainingTime(n);
    for (int i = 0; i < n; i++) {
        remainingTime[i] = proc[i].burst_time;
    }

    int currentTime = 0, completed = 0, shortest = -1, highestPriority = 9999;

    while (completed < n) {
        highestPriority = INT_MIN;
        shortest = -1;

        // Find the process with the highest priority (smallest priority value)
        for (int i = 0; i < n; i++) {


            if (proc[i].arrival_time <= currentTime && remainingTime[i] > 0 && proc[i].priority > highestPriority) {
                highestPriority = proc[i].priority;
                shortest = i;
            }
        }

        if (shortest == -1) {
            currentTime++;
            continue;
        }

        // Process the selected process
        remainingTime[shortest]--;
        currentTime++;

        // If the process is completed
        if (remainingTime[shortest] == 0) {
            
            completed++;
            proc[shortest].completion_time = currentTime;
            proc[shortest].turnaround_time = proc[shortest].completion_time - proc[shortest].arrival_time;
            proc[shortest].waiting_time = proc[shortest].turnaround_time - proc[shortest].burst_time;
        }
    }
}


void displayProcessDetails(Process proc[], int n) {
    cout << "PID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n";
    for (int i = 0; i < n; i++) {
        cout << proc[i].pid << "\t" << proc[i].arrival_time << "\t\t"
             << proc[i].burst_time << "\t\t" << proc[i].completion_time << "\t\t"
             << proc[i].turnaround_time << "\t\t" << proc[i].waiting_time << endl;
    }
    cout << endl;
}


void displayGanttChart(Process proc[], int n) {
    int start_times[n];

    // Calculate start times for each process
    start_times[0] = proc[0].arrival_time;
    for (int i = 1; i < n; i++) {
        start_times[i] = max(proc[i - 1].completion_time, proc[i].arrival_time);
    }

    // Display Gantt chart
    cout << "Gantt Chart:\n";

    // Top bar with process IDs
    cout << "|";
    for (int i = 0; i < n; i++) {
        cout << " P" << proc[i].pid << " |";
    }
    cout << "\n";

    // Bottom bar with time markers
    cout << start_times[0];
    for (int i = 0; i < n; i++) {
        int spacing = to_string(proc[i].completion_time).length() + 3; // Adjust for spacing
        cout << string(spacing, ' ') << proc[i].completion_time;
    }
    cout << "\n\n";
}


void details(Process proc[], int n, bool check) {
   
    if(check == false) {
    cout << "Enter Process Details (PID, Arrival Time, Burst Time):\n";
    for (int i = 0; i < n; i++) {
        cout << "Process " << i + 1 << ": ";
        proc[i].pid = i;
        cin >>proc[i].pid >> proc[i].arrival_time >> proc[i].burst_time;
       
       
    }
    }
    else if(check == true) {
    cout << "Enter Process Details (PID, Arrival Time, Burst Time, Priority):\n";
    for (int i = 0; i < n; i++) {
        cout << "Process " << i + 1 << ": ";
        proc[i].pid = i;
        cin >>proc[i].pid >> proc[i].arrival_time >> proc[i].burst_time >> proc[i].priority;
    }

    }
}

int main() {
    int choice, n;

    cout << "Enter the number of processes: ";
    cin >> n;

    Process proc[n];
  
    cout << "Choose Scheduling Algorithm:\n";
    cout << "1. FCFS\n";
    cout << "2. SJF\n";
    cout << "3. RR\n";
    cout << "4. PR\n";

    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        details(proc, n, false);
        findFCFS(proc, n);
        cout << "\nFCFS Scheduling:\n";
        displayProcessDetails(proc, n);
        displayGanttChart(proc, n);
    } else if (choice == 2) {
        details(proc, n, false);
        findSJF(proc, n);
        cout << "\nSJF Scheduling:\n";
        displayProcessDetails(proc, n);
        displayGanttChart(proc, n);
    } else if (choice == 3) {
        details(proc, n, false);
        findSJF(proc, n);
        cout << "\nRR Scheduling:\n";
        displayProcessDetails(proc, n);
        displayGanttChart(proc, n);
    } else if (choice == 4) {
        details(proc, n, true);
        findPriorityPreemptive(proc, n);
        cout << "\nPriority Scheduling:\n";
        displayProcessDetails(proc, n);
        displayGanttChart(proc, n);
    }
    else {
        cout << "Invalid choice!\n";
    }
    return 0;
}
