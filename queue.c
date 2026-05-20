#include "header.h"

// ready queue에 프로세스 추가
void Enqueue_Ready(Process* ready_queue[], int* ready_count, Process* proc) {
    proc->state = STATE_READY;
    ready_queue[*ready_count] = proc;
    (*ready_count)++;
}

// ready queue에서 프로세스 꺼내기
Process* Dequeue_Ready(Process* ready_queue[], int* ready_count, int target_idx) {
    if (*ready_count == 0) return NULL;

    Process* target = ready_queue[target_idx];

    for (int i = target_idx + 1; i < *ready_count; i++) ready_queue[i - 1] = ready_queue[i];
    (*ready_count)--;

    return target;
}

// waiting queue에 프로세스 추가
void Enqueue_Waiting(Process* waiting_queue[], int* waiting_count, Process* proc) {
    proc->state = STATE_WAITING;
    waiting_queue[*waiting_count] = proc;
    (*waiting_count)++;
}

// waiting queue에서 프로세스 꺼내기
void Dequeue_Waiting_Process(Process* waiting_queue[], int* waiting_count, int target_pid) {
    int target_idx = -1;

    // 타겟 프로세스 인덱스 탐색
    for (int i = 0; i < *waiting_count; i++) {
        if (waiting_queue[i]->pid == target_pid) {
            target_idx = i;
            break;
        }
    }

    if (target_idx != -1) {
        for (int i = target_idx + 1; i < *waiting_count; i++) waiting_queue[i - 1] = waiting_queue[i];
        (*waiting_count)--;
    }

}

// FCFS
Process* Dequeue_Ready_FCFS(Process* ready_queue[], int* ready_count) {
    if (*ready_count == 0) return NULL;
    return Dequeue_Ready(ready_queue, ready_count, 0);
}

// SJF
Process* Dequeue_Ready_SJF(Process* ready_queue[], int* ready_count) {
    if (*ready_count == 0) return NULL;

    int best_idx = 0;

    for (int i = 1; i < *ready_count; i++) {
        // 1. 남은 cpu 시간이 더 짧은 것 우선
        if (ready_queue[i]->remaining_cpu < ready_queue[best_idx]->remaining_cpu) best_idx = i;
        // 2. 남은 cpu 시간이 같다면 먼저 도착한 것 우선
        else if (ready_queue[i]->remaining_cpu == ready_queue[best_idx]->remaining_cpu) {
            if (ready_queue[i]->arrival_time < ready_queue[best_idx]->arrival_time) best_idx = i;
            // 3. 도착한 시간이 같다면 pid가 작은 것 우선
            else if (ready_queue[i]->arrival_time == ready_queue[best_idx]->arrival_time) {
                if (ready_queue[i]->pid < ready_queue[best_idx]->pid) best_idx = i;
            }
        }
    }

    return Dequeue_Ready(ready_queue, ready_count, best_idx);
}

// Priority
Process* Dequeue_Ready_Priority(Process* ready_queue[], int* ready_count) {
    if (*ready_count == 0) return NULL;

    int best_idx = 0;

    for (int i = 1; i < *ready_count; i++) {
        // 1. 더 높은 우선순위 우선
        if (ready_queue[i]->priority > ready_queue[best_idx]->priority) best_idx = i;
        // 2. 우선순위가 같다면 먼저 도착한 것 우선
        else if (ready_queue[i]->priority == ready_queue[best_idx]->priority) {
            if (ready_queue[i]->arrival_time < ready_queue[best_idx]->arrival_time) best_idx = i;
            // 3. 도착한 시간이 같다면 pid가 작은 것 우선
            else if (ready_queue[i]->arrival_time == ready_queue[best_idx]->arrival_time) {
                if (ready_queue[i]->pid < ready_queue[best_idx]->pid) best_idx = i;
            }
        }
    }

    return Dequeue_Ready(ready_queue, ready_count, best_idx);
}