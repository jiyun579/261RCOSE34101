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
void Dequeue_Waiting(Process* waiting_queue[], int* waiting_count, int target_pid) {
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