#include "header.h"

int selected_algorithm = 0;
int time_quantum = 0;

void Config() {

    printf("\n[ Settings ] Which algorithm would you like to run?\n");
    printf(" 1. FCFS (First Come First Served)\n");
    printf(" 2. SJF (Shortest Job First) - Non-Preemptive\n");
    printf(" 3. SJF (Shortest Job First) - Preemptive\n");
    printf(" 4. Priority Scheduling - Non-Preemptive\n");
    printf(" 5. Priority Scheduling - Preemptive\n");
    printf(" 6. RR (Round Robin)\n");
    
    // 1-1. 알고리즘 선택
    while (1) {
        printf("\n>>> Enter the algorithm number to perform. (1 ~ 6): ");
        if (scanf("%d", &selected_algorithm) == 1) {
            if (selected_algorithm >= 1 && selected_algorithm <= 6) {
                break; 
            }
        }
        while (getchar() != '\n'); // 입력 버퍼 비움
        printf("\n[ Warning ] Invalid input. Please enter a number between 1 and 6.\n");
    }

    // 1-2. Round Robin을 선택한 경우
    if (selected_algorithm == 6) {
        while (1) {
            printf("\n>>> Enter the time quantum for Round Robin. (2 ~ 5): ");
            if (scanf("%d", &time_quantum) == 1 && time_quantum > 0) {
                if (time_quantum < 2 || time_quantum > 5) {
                printf("\n[ Warning ] For realistic simulation, please enter a number between 2 and 5.\n");
                continue;
            }
                break;
            }
            while (getchar() != '\n'); // 입력 버퍼 비움
            printf("\n[ Warning ] Invalid input. Please enter a natural number greater than 0.\n");
        }
    }

    // 2. 알고리즘 선택 결과 출력
    if (selected_algorithm == 6) {
        printf("\n[ Done ] Round Robin, Time Quantum: %d sec\n", time_quantum);
    } 
    else {
        const char* algo_names[] = {"", "FCFS", "Non-Preemptive SJF", "Preemptive SJF", "Non-Preemptive Priority", "Preemptive Priority"};
        printf("\n[ Done ] %s\n", algo_names[selected_algorithm]);
    }
}

int main() {

    srand((unsigned int)time(NULL));

    // 1. 프로세스 개수 설정
    int process_count = 0;
    while (1) {
        printf("\n>>> Enter the number of processes you want to create (1 ~ 10): ");
        if (scanf("%d", &process_count) == 1 && process_count > 0) {
            if (process_count > 10) {
                printf("\n[ Warning ] Too many processes. Maximum limit is 10.\n");
                continue;
            }
            break;
        }
        while (getchar() != '\n'); // 입력 버퍼 비움
        printf("\n[ Warning ] Invalid input. Please enter a positive integer.\n");
    }

    // 2. 프로세스 공간 할당
    Process** proc_list = (Process**)malloc(sizeof(Process*) * process_count);
    if (proc_list == NULL) {
        printf("\n[ Error ] Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < process_count; i++) {
        proc_list[i] = (Process*)malloc(sizeof(Process));
        if (proc_list[i] == NULL) {
            printf("\n[ Error ] Memory allocation failed at index %d.\n", i);
            return 1;
        }
        memset(proc_list[i], 0, sizeof(Process));
    }

    // 3. 프로세스 생성 및 출력
    Create_Process(proc_list, process_count);
    Print_Process_List(proc_list, process_count);

    // 4. 알고리즘 설정
    while (1) {
 
        Config();

        // 시뮬레이션 시작 질의 (y/n)
        char choice;
        while (1) {
            printf("\n>>> Would you like to start the simulation? (y/n): ");
            while (getchar() != '\n'); // 입력 버퍼 비움
            scanf("%c", &choice);

            if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N') break;
            printf("\n[ Warning ] Invalid choice. Please enter 'y' or 'n'.\n");
        }

        // y인 경우
        if (choice == 'y' || choice == 'Y') {
            switch (selected_algorithm) {
                case 1: FCFS(proc_list, process_count); break;
                case 2: SJF_Non_Preemptive(proc_list, process_count); break;
                case 3: SJF_Preemptive(proc_list, process_count); break;
                case 4: Priority_Non_Preemptive(proc_list, process_count); break;
                case 5: Priority_Preemptive(proc_list, process_count); break;
                case 6: RR(proc_list, process_count, time_quantum); break;
            }
            break;
        }

        // n인 경우
        if (choice == 'n' || choice == 'N') {
            int sub_choice = 0;
            printf("\n[ Done ] Simulation canceled. What would you like to do?\n");
            printf(" 1. Change Settings (Re-select Algorithm)\n");
            printf(" 2. Terminate Program\n");
            
            while (1) {
                printf("\n>>> Enter option number (1 or 2): ");
                if (scanf("%d", &sub_choice) == 1 && (sub_choice == 1 || sub_choice == 2)) break;
                while (getchar() != '\n'); // 입력 버퍼 비움
                printf("\n[ Warning ] Invalid input. Please enter 1 or 2.\n");
            }

            if (sub_choice == 1) {
                printf("\n[ Done ] Return to configuration settings.\n");
                continue; 
            } else if (sub_choice == 2) {
                printf("\n[ Done ] Program terminated by user.\n\n");
                break;
            }
        }
    }

    for (int i = 0; i < process_count; i++) free(proc_list[i]);
    free(proc_list);

    return 0;
}