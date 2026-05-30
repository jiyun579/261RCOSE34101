#include "header.h"
#define TIMEOUT_LIMIT 10000
#define GANTT_SIZE 10005

void FCFS(Process* proc_list[], int process_count) {
    int current_time = 0;
    int terminated_count = 0;
    int gantt[GANTT_SIZE] = {0};
    
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    Process* running_proc = NULL;

    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리 (Waiting -> Ready)
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];

            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting(waiting_queue, &waiting_count, waiting_proc->pid);
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 실행 중인 프로세스가 없다면 스케줄링 (Ready -> Running)
        if (running_proc == NULL && ready_count > 0) {
            running_proc = Dequeue_Ready(ready_queue, &ready_count, 0);
            running_proc->state = STATE_RUNNING;
        }

        // 4. 정보 업데이트
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_READY) proc_list[i]->waiting_time++;
        } 
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
        }

        // 5. CPU burst 실행
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;

            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 종료 검사 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0 && running_proc->remaining_io == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time + 1;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                running_proc = NULL;
                terminated_count++;
            }
            // I/O 발생 검사 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0) {
                if (executed_cpu % running_proc->io_interval == 0) {
                    running_proc->current_io = 2;
                    running_proc->remaining_io -= 2;
                    running_proc->remaining_io_count--;
                    
                    Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                    running_proc = NULL;
                }
            }
        } 
        else gantt[current_time] = 0;

        current_time++;
        if (current_time >= TIMEOUT_LIMIT) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] FCFS Simulation Finished.\n");
    Gantt_Chart(proc_list, process_count, gantt, current_time);

    free(ready_queue);
    free(waiting_queue);
}


void Non_Preemptive_SJF(Process* proc_list[], int process_count) {
    int current_time = 0;
    int terminated_count = 0;
    int gantt[GANTT_SIZE] = {0};
    
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    Process* running_proc = NULL;

    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리 (Waiting -> Ready)
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];

            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting(waiting_queue, &waiting_count, waiting_proc->pid);
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 실행 중인 프로세스가 없다면 스케줄링 (Ready -> Running)
        if (running_proc == NULL && ready_count > 0) {
            int target_idx = Best_SJF_Index(ready_queue, ready_count);
            running_proc = Dequeue_Ready(ready_queue, &ready_count, target_idx);
            running_proc->state = STATE_RUNNING;
        }

        // 4. 정보 업데이트
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_READY) proc_list[i]->waiting_time++;
        } 
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
        }

        // 5. CPU burst 실행
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;

            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 종료 검사 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0 && running_proc->remaining_io == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time + 1;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                running_proc = NULL;
                terminated_count++;
            }
            // I/O 발생 검사 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0) {
                if (executed_cpu % running_proc->io_interval == 0) {
                    running_proc->current_io = 2;
                    running_proc->remaining_io -= 2;
                    running_proc->remaining_io_count--;
                    
                    Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                    running_proc = NULL;
                }
            }
        } 
        else gantt[current_time] = 0;

        current_time++;
        if (current_time >= TIMEOUT_LIMIT) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] Non-Preemptive SJF Simulation Finished.\n");
    Gantt_Chart(proc_list, process_count, gantt, current_time);

    free(ready_queue);
    free(waiting_queue);
}


void Preemptive_SJF(Process* proc_list[], int process_count) {
    int current_time = 0;
    int terminated_count = 0;
    int gantt[GANTT_SIZE] ={0};
    
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    Process* running_proc = NULL;

    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리 (Waiting -> Ready)
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];

            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting(waiting_queue, &waiting_count, waiting_proc->pid);
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 선점형 기능 처리 (Ready <-> Running)
        if (ready_count > 0) {
            int target_idx = Best_SJF_Index(ready_queue, ready_count);

            if (running_proc != NULL) {
                if (ready_queue[target_idx]->remaining_cpu < running_proc->remaining_cpu) {
                    Enqueue_Ready(ready_queue, &ready_count, running_proc);
                    running_proc = NULL;
                }
            } 
        }

        if (running_proc == NULL && ready_count > 0) {
            int target_idx = Best_SJF_Index(ready_queue, ready_count);
            running_proc = Dequeue_Ready(ready_queue, &ready_count, target_idx);
            running_proc->state = STATE_RUNNING;
        }

        // 4. 정보 업데이트
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_READY) proc_list[i]->waiting_time++;
        } 
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
        }

        // 5. CPU burst 실행
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;

            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 종료 검사 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0 && running_proc->remaining_io == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time + 1;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                running_proc = NULL;
                terminated_count++;
            }
            // I/O 발생 검사 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0) {
                if (executed_cpu % running_proc->io_interval == 0) {
                    running_proc->current_io = 2;
                    running_proc->remaining_io -= 2;
                    running_proc->remaining_io_count--;
                    
                    Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                    running_proc = NULL;
                }
            }
        } 
        else gantt[current_time] = 0;

        current_time++;
        if (current_time >= TIMEOUT_LIMIT) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] Preemptive SJF Simulation Finished.\n");
    Gantt_Chart(proc_list, process_count, gantt, current_time);

    free(ready_queue);
    free(waiting_queue);
}


void Non_Preemptive_Priority(Process* proc_list[], int process_count){
    int current_time = 0;
    int terminated_count = 0;
    int gantt[GANTT_SIZE] = {0};
    
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    Process* running_proc = NULL;

    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리 (Waiting -> Ready)
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];

            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting(waiting_queue, &waiting_count, waiting_proc->pid);
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 실행 중인 프로세스가 없다면 스케줄링 (Ready -> Running)
        if (running_proc == NULL && ready_count > 0) {
            int target_idx = Best_Priority_Index(ready_queue, ready_count);
            running_proc = Dequeue_Ready(ready_queue, &ready_count, target_idx);
            running_proc->state = STATE_RUNNING;
        }

        // 4. 정보 업데이트
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_READY) proc_list[i]->waiting_time++;
        } 
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
        }

        // 5. CPU burst 실행
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;

            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 종료 검사 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0 && running_proc->remaining_io == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time + 1;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                running_proc = NULL;
                terminated_count++;
            }
            // I/O 발생 검사 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0) {
                if (executed_cpu % running_proc->io_interval == 0) {
                    running_proc->current_io = 2;
                    running_proc->remaining_io -= 2;
                    running_proc->remaining_io_count--;
                    
                    Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                    running_proc = NULL;
                }
            }
        } 
        else gantt[current_time] = 0;

        current_time++;
        if (current_time >= TIMEOUT_LIMIT) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] Non-Preemptive Priority Simulation Finished.\n");
    Gantt_Chart(proc_list, process_count, gantt, current_time);

    free(ready_queue);
    free(waiting_queue);
}


void Preemptive_Priority(Process* proc_list[], int process_count){
    int current_time = 0;
    int terminated_count = 0;
    int gantt[GANTT_SIZE] = {0};
    
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    Process* running_proc = NULL;

    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리 (Waiting -> Ready)
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];

            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting(waiting_queue, &waiting_count, waiting_proc->pid);
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 선점형 기능 처리 (Ready <-> Running)
        if (ready_count > 0) {
            int target_idx = Best_Priority_Index(ready_queue, ready_count);

            if (running_proc != NULL) {
                if (ready_queue[target_idx]->priority > running_proc->priority) {
                    Enqueue_Ready(ready_queue, &ready_count, running_proc);
                    running_proc = NULL;
                }
            } 
        }

        if (running_proc == NULL && ready_count > 0) {
            int target_idx = Best_Priority_Index(ready_queue, ready_count);
            running_proc = Dequeue_Ready(ready_queue, &ready_count, target_idx);
            running_proc->state = STATE_RUNNING;
        }

        // 4. 정보 업데이트
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_READY) proc_list[i]->waiting_time++;
        } 
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
        }

        // 5. CPU burst 실행
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;

            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 종료 검사 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0 && running_proc->remaining_io == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time + 1;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                running_proc = NULL;
                terminated_count++;
            }
            // I/O 발생 검사 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0) {
                if (executed_cpu % running_proc->io_interval == 0) {
                    running_proc->current_io = 2;
                    running_proc->remaining_io -= 2;
                    running_proc->remaining_io_count--;
                    
                    Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                    running_proc = NULL;
                }
            }
        } 
        else gantt[current_time] = 0;

        current_time++;
        if (current_time >= TIMEOUT_LIMIT) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] Preemptive Priority Simulation Finished.\n");
    Gantt_Chart(proc_list, process_count, gantt, current_time);

    free(ready_queue);
    free(waiting_queue);
}


void RR(Process* proc_list[], int process_count, int time_quantum){
    int current_time = 0;
    int terminated_count = 0;
    int gantt[GANTT_SIZE] = {0};
    
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;
    Process** waiting_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int waiting_count = 0;
    Process* running_proc = NULL;

    while (terminated_count < process_count) {

        // 1. I/O 대기 프로세스 처리 (Waiting -> Ready)
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];

            if (waiting_proc->current_io <= 0) {
                Dequeue_Waiting(waiting_queue, &waiting_count, waiting_proc->pid);
                waiting_proc->time_slice = 0;
                Enqueue_Ready(ready_queue, &ready_count, waiting_proc);
            }
        }

        // 2. 도착 프로세스 처리 (New -> Ready)
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_NEW && proc_list[i]->arrival_time == current_time) {
                proc_list[i]->time_slice = 0;
                Enqueue_Ready(ready_queue, &ready_count, proc_list[i]);
            }
        }

        // 3. 실행 중인 프로세스가 없다면 스케줄링 (Ready -> Running)
        if (running_proc == NULL && ready_count > 0) {
            running_proc = Dequeue_Ready(ready_queue, &ready_count, 0);
            running_proc->state = STATE_RUNNING;
            running_proc->time_slice = 0;
        }

        // 4. 정보 업데이트
        for (int i = 0; i < process_count; i++) {
            if (proc_list[i]->state == STATE_READY) proc_list[i]->waiting_time++;
        } 
        for (int i = waiting_count - 1; i >= 0; i--) {
            Process* waiting_proc = waiting_queue[i];
            waiting_proc->current_io--;
        }

        // 5. CPU burst 실행
        if (running_proc != NULL) {
            gantt[current_time] = running_proc->pid;
            running_proc->remaining_cpu--;
            running_proc->time_slice++;

            int executed_cpu = running_proc->cpu_burst - running_proc->remaining_cpu;

            // 종료 검사 (Running -> Terminated)
            if (running_proc->remaining_cpu == 0 && running_proc->remaining_io == 0) {
                running_proc->state = STATE_TERMINATED;
                running_proc->terminated_time = current_time + 1;
                running_proc->turn_around_time = running_proc->terminated_time - running_proc->arrival_time;
                running_proc = NULL;
                terminated_count++;
            }
            // I/O 발생 검사 (Running -> Waiting)
            else if (running_proc->remaining_io_count > 0 && executed_cpu > 0 && (executed_cpu % running_proc->io_interval == 0)) {
                running_proc->time_slice = 0;
                running_proc->current_io = 2;
                running_proc->remaining_io -= 2;
                running_proc->remaining_io_count--;
                
                Enqueue_Waiting(waiting_queue, &waiting_count, running_proc);
                running_proc = NULL;
            }
            // time quantum 검사
            else if (running_proc->time_slice == time_quantum) {
                running_proc->time_slice = 0;
                Enqueue_Ready(ready_queue, &ready_count, running_proc);
                running_proc = NULL;
            }
        } 
        else gantt[current_time] = 0;

        current_time++;
        if (current_time >= TIMEOUT_LIMIT) {
            printf("\n[ Error ] Simulation timeout.\n");
            break;
        }
    }

    printf("\n[ Done ] Round Robin Simulation Finished.\n");
    Gantt_Chart(proc_list, process_count, gantt, current_time);

    free(ready_queue);
    free(waiting_queue);
}


void Run_All_Algorithms(Process* original_proc_list[], int process_count, int time_quantum) {
    Process** sim_proc_list = (Process**)malloc(sizeof(Process*) * process_count);
    for (int i = 0; i < process_count; i++) {
        sim_proc_list[i] = (Process*)malloc(sizeof(Process));
    }

    printf("\n[ 1/6 ] Running FCFS Simulation...");
    Reset_Process_List(sim_proc_list, original_proc_list, process_count);
    FCFS(sim_proc_list, process_count);
    printf("---------------------------------------------------------------------------\n");

    printf("\n[ 2/6 ] Running Non-Preemptive SJF Simulation...");
    Reset_Process_List(sim_proc_list, original_proc_list, process_count);
    Non_Preemptive_SJF(sim_proc_list, process_count);
    printf("---------------------------------------------------------------------------\n");

    printf("\n[ 3/6 ] Running Preemptive SJF Simulation...");
    Reset_Process_List(sim_proc_list, original_proc_list, process_count);
    Preemptive_SJF(sim_proc_list, process_count);
    printf("---------------------------------------------------------------------------\n");

    printf("\n[ 4/6 ] Running Non-Preemptive Priority Simulation...");
    Reset_Process_List(sim_proc_list, original_proc_list, process_count);
    Non_Preemptive_Priority(sim_proc_list, process_count);
    printf("---------------------------------------------------------------------------\n");

    printf("\n[ 5/6 ] Running Preemptive Priority Simulation...");
    Reset_Process_List(sim_proc_list, original_proc_list, process_count);
    Preemptive_Priority(sim_proc_list, process_count);
    printf("---------------------------------------------------------------------------\n");

    printf("\n[ 6/6 ] Running Round Robin Simulation (Time Quantum: %d)...", time_quantum);
    Reset_Process_List(sim_proc_list, original_proc_list, process_count);
    RR(sim_proc_list, process_count, time_quantum);
    
    printf("\n[ Done ] All Simulations Finished.\n\n");

    for (int i = 0; i < process_count; i++) free(sim_proc_list[i]);
    free(sim_proc_list);
}