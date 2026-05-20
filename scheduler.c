#include "header.h"

void FCFS(Process* proc_list[], int process_count) {

    int current_time = 0;
    int terminated_count = 0;
    int gantt[10000];
    
    // 큐 공간 동적 할당 (프로세스 개수만큼)
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    
    Process* running_proc = NULL; // 현재 CPU를 선점한 프로세스

    // 모든 프로세스가 종료될 때까지 루프
    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리
        for (int i = waiting_count-1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
            
            // I/O가 종료된 경우 (Waiting -> Ready)
            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting_Process(waiting_queue, &waiting_count, waiting_proc->pid);
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 실행 중인 프로세스 처리
        if (running_proc != NULL) {
            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 프로세스가 종료된 경우 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                
                running_proc = NULL;
                terminated_count++;
            }
            // I/O를 수행해야 하는 경우 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0) {
                if (executed_cpu % running_proc->io_interval == 0) {
                    running_proc->current_io = running_proc->remaining_io / running_proc->remaining_io_count;
                    running_proc->remaining_io -= running_proc->current_io;
                    running_proc->remaining_io_count--;
                    
                    Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                    running_proc = NULL;
                }
            }
        }

        // 4. 실행 중인 프로세스가 없는 경우 (Ready -> Running)
        if (running_proc == NULL && ready_count > 0) {
            running_proc = Dequeue_Ready_FCFS(ready_queue, &ready_count);
            running_proc->state = STATE_RUNNING;
        }

        // 5. 정보 업데이트
        for (int i = 0; i < ready_count; i++) ready_queue[i]->waiting_time++;
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;
        } 
        else gantt[current_time] = 0;

        current_time++;
        
        // 무한 루프 방지
        if (current_time > 10000) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] FCFS Simulation Finished.\n");
    gantt_chart(proc_list, process_count, gantt);

    free(ready_queue);
    free(waiting_queue);
}


void SJF_Non_Preemptive(Process* proc_list[], int process_count){

}


void SJF_Preemptive(Process* proc_list[], int process_count){

}


void Priority_Non_Preemptive(Process* proc_list[], int process_count){

}


void Priority_Preemptive(Process* proc_list[], int process_count){

}


void RR(Process* proc_list[], int process_count, int time_quantum){
    
}