#include "header.h"

int selected_algorithm = 0;
int time_quantum = 0;

void Config() {

    printf("\n==========================================================\n");
    printf("          CPU Scheduling Simulator Configuration          \n");
    printf("==========================================================\n");
    printf(" 1. FCFS (First Come First Served)\n");
    printf(" 2. SJF (Shortest Job First) - Non-Preemptive\n");
    printf(" 3. SJF (Shortest Job First) - Preemptive\n");
    printf(" 4. Priority Scheduling - Non-Preemptive\n");
    printf(" 5. Priority Scheduling - Preemptive\n");
    printf(" 6. RR (Round Robin)\n");
    printf("==========================================================\n");
    
    // 알고리즘 선택
    while (1) {
        printf("Enter the algorithm number to perform. (1 ~ 6): ");
        if (scanf("%d", &selected_algorithm) == 1) {
            if (selected_algorithm >= 1 && selected_algorithm <= 6) {
                break; 
            }
        }

        while (getchar() != '\n'); // 입력 버퍼 비움
        printf("[Invalid input! Please enter a number between 1 and 6.]\n");
    }

    // Round Robin을 선택한 경우
    if (selected_algorithm == 6) {
        while (1) {
            printf("Enter the time quantum for Round Robin. (Recommended: 2 to 5): ");
            if (scanf("%d", &time_quantum) == 1 && time_quantum > 0) {
                break;
            }

            while (getchar() != '\n'); // 입력 버퍼 비움
            printf("[Invalid input! Please enter a natural number greater than 0.]\n");
        }
    }

    printf("[System configuration complete!]\n");
    if (selected_algorithm == 6) {
        printf("[Set up Results] Algorithm: Round Robin, Time Quantum: %d sec\n", time_quantum);
    } 
    else {
        const char* algo_names[] = {"", "FCFS", "Non-Preemptive SJF", "Preemptive SJF", "Non-Preemptive Priority", "Preemptive Priority"};
        printf("[Set up Results] Algorithm: %s\n", algo_names[selected_algorithm]);
    }
    printf("==========================================================\n\n");
}