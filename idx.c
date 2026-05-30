#include "header.h"

int Best_SJF_Index(Process* ready_queue[], int ready_count) {
    if (ready_count == 0) return -1;

    int best_idx = 0;
    for (int i = 1; i < ready_count; i++) {
        // 1. 남은 cpu 시간이 더 짧은 것 우선
        if (ready_queue[i]->remaining_cpu < ready_queue[best_idx]->remaining_cpu)best_idx = i;
        else if (ready_queue[i]->remaining_cpu == ready_queue[best_idx]->remaining_cpu) {
            // 2. 남은 cpu 시간이 같다면 pid가 작은 것 우선
            if (ready_queue[i]->pid < ready_queue[best_idx]->pid) best_idx = i;
        }
    }
    return best_idx;
}

int Best_Priority_Index(Process* ready_queue[], int ready_count) {
    if (ready_count == 0) return -1;

    int best_idx = 0;
    for (int i = 1; i < ready_count; i++) {
        // 1. 더 높은 우선순위 우선
        if (ready_queue[i]->priority > ready_queue[best_idx]->priority) best_idx = i;
        else if (ready_queue[i]->priority == ready_queue[best_idx]->priority) {
            // 2. 우선순위가 같다면 pid가 작은 것 우선
            if (ready_queue[i]->pid < ready_queue[best_idx]->pid) best_idx = i;
        }
    }
    return best_idx;
}