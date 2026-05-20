#include "header.h"

// 프로세스 생성
void Create_Process(Process* proc_list[], int count) {
    
    for (int i = 0; i < count; i++) {

        // 1. 기본 정보
        proc_list[i]->pid = i + 1;                      // 1부터 순차 부여
        proc_list[i]->arrival_time = rand() % 11;       // 도착 시간: 0 ~ 10 랜덤
        proc_list[i]->priority = (rand() % 10) + 1;     // 우선순위: 1 ~ 10 랜덤
        proc_list[i]->state = STATE_NEW;                // 초기: 대기 상태

        // 2. CPU 정보
        proc_list[i]->cpu_burst = (rand() % 12) + 4;    // CPU 총 시간: 4 ~ 15 랜덤
        proc_list[i]->remaining_cpu = proc_list[i]->cpu_burst;

        // 3. I/O 정보
        int dice = rand() % 10;
        if (dice < 3) proc_list[i]->io_count = 0;       // 30% 확률로 I/O 0회
        else if (dice < 7) proc_list[i]->io_count = 1;  // 40% 확률로 I/O 1회
        else proc_list[i]->io_count = 2;                // 30% 확률로 I/O 2회
        proc_list[i]->remaining_io_count = proc_list[i]->io_count;
        // I/O가 발생하는 경우
        if (proc_list[i]->io_count > 0) {
            proc_list[i]->io_burst = (rand() % 4) + 2; // I/O 총 시간: 2 ~ 5 랜덤
            proc_list[i]->current_io = 0;
            proc_list[i]->remaining_io = proc_list[i]->io_burst;
            proc_list[i]->io_interval = proc_list[i]->cpu_burst / (proc_list[i]->io_count + 1); // I/O 실행 간격: CPU 시간에 따라 균등한 간격
            if (proc_list[i]->io_interval == 0) proc_list[i]->io_interval = 1;
        }
        // I/O가 발생하지 않는 경우
        else {
            proc_list[i]->io_burst = 0;
            proc_list[i]->current_io = 0;
            proc_list[i]->remaining_io = 0;
            proc_list[i]->io_interval = 0;
        }

        // 4. 평가 정보
        proc_list[i]->turn_around_time = 0;
        proc_list[i]->waiting_time = 0;
    }
}

// 프로세스 출력
void Print_Process_List(Process* proc_list[], int count) {
    printf("\n[Done] Process created.\n");
    printf("=================================================================================\n");
    printf(" PID   |   Arrival   |   CPU_Burst   |   Priority   |   I/O_Count   |   I/O_Burst\n");
    printf("---------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf(" P%-3d  |%7d      |%8d       |%8d      |%8d       |%8d\n",
                proc_list[i]->pid,
                proc_list[i]->arrival_time,
                proc_list[i]->cpu_burst,
                proc_list[i]->priority,
                proc_list[i]->io_count,
                proc_list[i]->io_burst);
    }
    printf("=================================================================================\n");
}