#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function prototypes
void fcfs_disk(int requests[], int n, int head);
void scan_disk(int requests[], int n, int head, int disk_size);
void cscan_disk(int requests[], int n, int head, int disk_size);
void sort_requests(int requests[], int n);

int main() {
    int choice, n, disk_size, head;

    // Input number of disk requests
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Number of requests must be positive.\n");
        return 1;
    }
    
    int requests[n];
    
    // Input disk requests
    printf("Enter the disk requests:\n");
    for (int i = 0; i < n; i++) {
        printf("Request %d: ", i + 1);
        scanf("%d", &requests[i]);
        
        // Validate requests
        if (requests[i] < 0) {
            printf("Request values must be non-negative.\n");
            return 1;
        }
    }

    // Input initial head position
    printf("Enter the initial head position: ");
    scanf("%d", &head);

    // Validate head position
    if (head < 0) {
        printf("Head position must be non-negative.\n");
        return 1;
    }

    // Input disk size
    printf("Enter the size of the disk: ");
    scanf("%d", &disk_size);

    // Validate disk size
    if (disk_size <= 0) {
        printf("Disk size must be positive.\n");
        return 1;
    }

    do {
        // Display menu
        printf("\nChoose Disk Scheduling Algorithm:\n");
        printf("1. FCFS\n");
        printf("2. SCAN\n");
        printf("3. C-SCAN\n");
        printf("4. Exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("First-Come-First-Served (FCFS) Disk Scheduling\n");
                fcfs_disk(requests, n, head);
                break;
            case 2:
                printf("SCAN Disk Scheduling\n");
                scan_disk(requests, n, head, disk_size);
                break;
            case 3:
                printf("C-SCAN Disk Scheduling\n");
                cscan_disk(requests, n, head, disk_size);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);

    return 0;
}

// FCFS Disk Scheduling
void fcfs_disk(int requests[], int n, int head) {
    int total_seek = 0;
    int current_position = head;

    printf("FCFS Scheduling Order:\n");
    printf("Head\tSeek\n");
    
    for (int i = 0; i < n; i++) {
        int seek = abs(requests[i] - current_position);
        printf("%d\t%d\n", current_position, seek);
        total_seek += seek;
        current_position = requests[i];
    }

    printf("Total Seek Time: %d\n", total_seek);
}

// SCAN Disk Scheduling
void scan_disk(int requests[], int n, int head, int disk_size) {
    int seek_sequence[n + 2];
    int direction, j = 0, total_seek = 0;

    // Sort requests
    sort_requests(requests, n);

    // Determine direction
    if (head < requests[0]) {
        direction = 1;
    } else if (head > requests[n - 1]) {
        direction = -1;
    } else {
        printf("Direction is ambiguous; choose direction manually.\n");
        printf("Enter direction (1 for up, -1 for down): ");
        scanf("%d", &direction);
    }

    // SCAN algorithm
    if (direction == 1) {
        seek_sequence[j++] = head;
        for (int i = 0; i < n && requests[i] < head; i++) {
            seek_sequence[j++] = requests[i];
        }
        seek_sequence[j++] = disk_size - 1;
        for (int i = n - 1; i >= 0 && requests[i] > head; i--) {
            seek_sequence[j++] = requests[i];
        }
    } else {
        seek_sequence[j++] = head;
        for (int i = n - 1; i >= 0 && requests[i] > head; i--) {
            seek_sequence[j++] = requests[i];
        }
        seek_sequence[j++] = 0;
        for (int i = 0; i < n && requests[i] < head; i++) {
            seek_sequence[j++] = requests[i];
        }
    }

    // Calculate total seek time
    printf("SCAN Scheduling Order:\n");
    printf("Head\tSeek\n");
    for (int i = 0; i < j - 1; i++) {
        int seek = abs(seek_sequence[i + 1] - seek_sequence[i]);
        printf("%d\t%d\n", seek_sequence[i], seek);
        total_seek += seek;
    }

    if (direction == 1 && head < disk_size - 1) {
        total_seek += (disk_size - 1 - head) + (disk_size - 1 - seek_sequence[j - 2]);
    }

    printf("Total Seek Time: %d\n", total_seek);
}

// C-SCAN Disk Scheduling
void cscan_disk(int requests[], int n, int head, int disk_size) {
    int seek_sequence[n + 2];
    int j = 0, total_seek = 0;

    // Sort requests
    sort_requests(requests, n);

    // C-SCAN algorithm
    seek_sequence[j++] = head;
    for (int i = 0; i < n && requests[i] < head; i++) {
        seek_sequence[j++] = requests[i];
    }
    if (head < disk_size - 1) {
        seek_sequence[j++] = disk_size - 1;
    }
    for (int i = 0; i < n && requests[i] >= head; i++) {
        seek_sequence[j++] = requests[i];
    }

    // Calculate total seek time
    printf("C-SCAN Scheduling Order:\n");
    printf("Head\tSeek\n");
    for (int i = 0; i < j - 1; i++) {
        int seek = abs(seek_sequence[i + 1] - seek_sequence[i]);
        printf("%d\t%d\n", seek_sequence[i], seek);
        total_seek += seek;
    }
    if (head < disk_size - 1) {
        total_seek += (disk_size - 1 - head) + (disk_size - 1 - seek_sequence[j - 2]);
    }

    printf("Total Seek Time: %d\n", total_seek);
}

// Helper function to sort requests
void sort_requests(int requests[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (requests[i] > requests[j]) {
                temp = requests[i];
                requests[i] = requests[j];
                requests[j] = temp;
            }
        }
    }
}
