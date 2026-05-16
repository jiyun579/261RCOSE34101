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
    int pid;
    int arrival_time;
    int priority;
    int cpu_burst;
    int io_burst;
    int io_start_time;

    ProcessState state;
    int remaining_cpu;
    int remaining_io;
    int io_trigger_count;

    int first_run_time;
    int response_time;
    int turn_around_time;
    int waiting_time;
    
    int completion_time;
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

// scheduler.c

// evaluation.c

#endif // SCHEDULER_H