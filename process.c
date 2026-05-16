#include "header.h"

// 프로세스 생성
void Create_Process(Process* proc_list[], int count) {
    
    for (int i = 0; i < count; i++) {

        // 기본 정보
        proc_list[i]->pid = i + 1;                       // 1부터 순차 부여
        proc_list[i]->arrival_time = rand() % 11;        // 도착 시간: 0 ~ 10 랜덤
        proc_list[i]->priority = (rand() % 10) + 1;      // 우선순위: 1 ~ 10 랜덤
        proc_list[i]->state = STATE_READY;               // 초기: 대기 상태

        // CPU 정보
        proc_list[i]->cpu_burst = (rand() % 16) + 5;     // CPU 총 시간: 5 ~ 20 랜덤
        proc_list[i]->remaining_cpu = proc_list[i]->cpu_burst;

        // I/O 정보
        int dice = rand() % 10;
        if (dice < 3) {
            proc_list[i]->io_count = 0;  // 30% 확률로 I/O 0회
        } 
        else if (dice < 7) {
            proc_list[i]->io_count = 1;  // 40% 확률로 I/O 1회
        } 
        else {
            proc_list[i]->io_count = 2;  // 30% 확률로 I/O 2회
        }

        if (proc_list[i]->io_count > 0) {
            proc_list[i]->io_burst = (rand() % 3) + 2; // I/O 총 시간: 2 ~ 4 랜덤
            proc_list[i]->remaining_io = proc_list[i]->io_burst;
            proc_list[i]->io_start = (rand() % (proc_list[i]->cpu_burst - 2)) + 1;
            proc_list[i]->io_end = -1;
        }
        else {
            proc_list[i]->io_burst = 0;
            proc_list[i]->remaining_io = 0;
            proc_list[i]->io_start = -1;
            proc_list[i]->io_end = -1;
        }

        // Criteria 정보
        proc_list[i]->turn_around_time = 0;
        proc_list[i]->waiting_time = 0;
    }
}

// 프로세스 출력 (확인용)
void Print_Process_List(Process* proc_list[], int count) {
    printf("=================================================================================\n");
    printf("PID     Arrival     CPU_Burst     Priority     I/O_Count     I/O_Burst\n");
    printf("=================================================================================\n");
    for (int i = 0; i < count; i++) {
        printf("%2d     %5d     %8d     %6d     %6d     %6d\n",
               proc_list[i]->pid, 
               proc_list[i]->arrival_time, 
               proc_list[i]->cpu_burst, 
               proc_list[i]->priority, 
               proc_list[i]->io_count,
               proc_list[i]->io_burst);
    }
}