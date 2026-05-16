#include "header.h"

void Create_Process(Process proc_list[], int count) {
    
    for (int i = 0; i < count; i++) {
        proc_list[i].pid = i + 1;                       // pid : 1부터 순차 부여
        proc_list[i].arrival_time = rand() % 11;        // 도착 시간: 0 ~ 10초 랜덤
        proc_list[i].priority = (rand() % 10) + 1;      // 우선순위: 1 ~ 10 랜덤
        proc_list[i].cpu_burst = (rand() % 16) + 5;     // CPU burst: 5 ~ 20초 랜덤

        // I/O 발생 횟수
        int dice = rand() % 10;
        if (dice < 3) {
            proc_list[i].io_trigger_count = 0; // 30% 확률로 I/O 없음
        } 
        else if (dice < 7) {
            proc_list[i].io_trigger_count = 1; // 40% 확률로 I/O 1회 발생
        } 
        else {
            proc_list[i].io_trigger_count = 2; // 30% 확률로 I/O 2회 발생
        }

        // I/O burst time
        if (proc_list[i].io_trigger_count > 0) {
            proc_list[i].io_burst = (rand() % 4) + 3;        // 총 I/O 시간: 3 ~ 6초
            
            // 첫 번째 I/O가 발생할 시점
            // cpu를 최소 1초 사용한 후 발생, 종료되기 2초 전엔 발생하도록 설정
            if (proc_list[i].cpu_burst > 3) {
                proc_list[i].io_start_time = (rand() % (proc_list[i].cpu_burst - 2)) + 1;
            } 
            else {
                proc_list[i].io_start_time = 1;
            }
        } 
        else {
            // I/O가 없는 프로세스 처리
            proc_list[i].io_burst = 0;
            proc_list[i].io_start_time = 0;
        }

        // 초기화 변수
        proc_list[i].state = STATE_READY;
        proc_list[i].remaining_cpu = proc_list[i].cpu_burst;
        proc_list[i].remaining_io = proc_list[i].io_burst;

        proc_list[i].turn_around_time = 0;
        proc_list[i].waiting_time = 0;
        proc_list[i].completion_time = 0;
    }
}


void Print_Process_List(Process proc_list[], int count) {
    printf("=================================================================================\n");
    printf("PID    Arrival    CPU_Burst    Priority    I/O_Burst    I/O_Start    I/O_Count\n");
    printf("=================================================================================\n");
    for (int i = 0; i < count; i++) {
        printf("%3d %5d sec %7d sec   %8d  %8d sec %8d sec   %8d\n",
               proc_list[i].pid, 
               proc_list[i].arrival_time, 
               proc_list[i].cpu_burst, 
               proc_list[i].priority, 
               proc_list[i].io_burst,
               proc_list[i].io_start_time,
               proc_list[i].io_trigger_count);
    }
}