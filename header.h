#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// 프로세스 상태
typedef enum {
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED
} ProcessState;

// 프로세스 정보
typedef struct {
    int pid; // 프로세스 ID (1부터 순차 부여)
    int arrival_time; // 도착 시간
    int priority; // 우선순위 (수가 클수록 높은 우선순위)
    ProcessState state; // 프로세스 상태

    int cpu_burst; // CPU 총 시간
    int remaining_cpu; // 남은 CPU 시간

    int io_count; // I/O 횟수
    int io_burst; // I/O 총 실행 시간
    int remaining_io; // 남은 I/O 시간
    int io_start; // 첫번째 I/O 시작까지 걸리는 시간 (CPU를 최소 1초 사용한 후 발생, 종료되기 2초 전엔 발생하도록 설정)
    int io_end; // 실행 중인 I/O 종료 시간 (실행 중이 아닐 때 -1)

    int turn_around_time; // 프로세스 도착 ~ 실행 완료까지 걸린 시간
    int waiting_time; // 프로세스 도착 후 실행 완료까지 총 대기 시간
} Process;

// 3. 글로벌 시스템 설정 변수 (다른 파일에서도 접근 가능하도록 extern 선언)
extern int selected_algorithm;
extern int time_quantum;

// main.c
void Config();

// process.c
void Create_Process(Process proc_list[], int count);
void Print_Process_List(Process proc_list[], int count);

// queue.c
void Enqueue_Ready(Process* ready_queue[], int* ready_count, Process* proc);
Process* Dequeue_Ready(Process* ready_queue[], int* ready_count, int target_idx);
void Enqueue_Waiting(Process* waiting_queue[], int* waiting_count, Process* proc);
void Dequeue_Waiting_Process(Process* waiting_queue[], int* waiting_count, int target_pid);
int Dequeue_idx_FCFS(Process* ready_queue[], int* ready_count);
int Dequeue_idx_SJF(Process* ready_queue[], int* ready_count);
int Dequeue_idx_Priority(Process* ready_queue[], int* ready_count);

// scheduler.c
void FCFS(Process* primary_processes, int process_count);

// evaluation.c

#endif // SCHEDULER_H