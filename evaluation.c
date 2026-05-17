#include "header.h"

// 간트 차트의 하나의 블록 정보를 담을 구조체 정의
typedef struct {
    int pid;
    int start_time;
    int end_time;
} GanttBlock;

void gantt_chart(Process* proc_list[], int process_count, int gantt_chart[]) {

    // 총 실행 시간
    int total_time = 0;
    for (int i = 0; i < process_count; i++) {
        if (proc_list[i]->terminated_time > total_time) {
            total_time = proc_list[i]->terminated_time;
        }
    }

    // GanttBlock
    GanttBlock blocks[1000];
    int block_count = 0;

    if (total_time > 0) {
        int current_pid = gantt_chart[0];
        int start_t = 0;

        for (int i = 1; i <= total_time; i++) {
            // 다음 초에 실행 종료 or 프로세스가 바뀌는 경우
            if (i == total_time || gantt_chart[i] != current_pid) {
                blocks[block_count].pid = current_pid;
                blocks[block_count].start_time = start_t;
                blocks[block_count].end_time = i;
                block_count++;

                if (i < total_time) {
                    current_pid = gantt_chart[i];
                    start_t = i;
                }
            }
        }
    }

    printf("\n[ Done ] Gantt Chart\n");

    int BLOCK_WIDTH = 6; 

    // 상단 테두리
    printf("  ");
    for (int i = 0; i < block_count; i++) {
        printf("+");
        for (int w = 0; w < BLOCK_WIDTH; w++) printf("-");
    }
    printf("+\n");

    // 실행 프로세스 출력
    printf("  ");
    for (int i = 0; i < block_count; i++) {
        printf("|");
        if (blocks[i].pid == 0) {
            printf(" IDLE ");
        } 
        else {
            // (PID 자릿수 대응을 위해 가변 공백 처리)
            char p_name[10];
            sprintf(p_name, "P%d", blocks[i].pid);
            int len = strlen(p_name);
            int total_spaces = BLOCK_WIDTH - len;
            int left_spaces = total_spaces / 2;
            int right_spaces = total_spaces - left_spaces;

            for(int s = 0; s < left_spaces; s++) printf(" ");
            printf("%s", p_name);
            for(int s = 0; s < right_spaces; s++) printf(" ");
        }
    }
    printf("|\n");

    // 하단 테두리
    printf("  ");
    for (int i = 0; i < block_count; i++) {
        printf("+");
        for (int w = 0; w < BLOCK_WIDTH; w++) printf("-");
    }
    printf("+\n");

    // 하단 타임라인
    printf("  ");
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].start_time < 10) {
            printf("%d", blocks[i].start_time);
            for (int w = 0; w < BLOCK_WIDTH; w++) printf(" ");
        } 
        else if (blocks[i].start_time < 100) {
            printf("%d", blocks[i].start_time);
            for (int w = 0; w < BLOCK_WIDTH - 1; w++) printf(" ");
        } 
        else {
            printf("%d", blocks[i].start_time);
            for (int w = 0; w < BLOCK_WIDTH - 2; w++) printf(" ");
        }
    }
    // 마지막 블록 종료 시간
    if (block_count > 0) {
        printf("%d", blocks[block_count - 1].end_time);
    }
    printf("\n");


    // 7. 개별 프로세스 통계 테이블 출력
    printf("\n[ Done ] Performence Evaluation\n");
    printf("===========================================================================\n");
    printf(" PID   |   Arrival   |   CPU_Burst   |   Waiting_Time   |   Turnaround_Time\n");
    printf("---------------------------------------------------------------------------\n");
    
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < process_count; i++) {
        printf(" P%-3d  |%7d      |%8d       |%10d        |%10d\n",
               proc_list[i]->pid,
               proc_list[i]->arrival_time,
               proc_list[i]->cpu_burst,
               proc_list[i]->waiting_time,
               proc_list[i]->turn_around_time);
        
        total_waiting_time += proc_list[i]->waiting_time;
        total_turnaround_time += proc_list[i]->turn_around_time;
    }
    printf("---------------------------------------------------------------------------\n");

    double avg_waiting = (double)total_waiting_time / process_count;
    double avg_turnaround = (double)total_turnaround_time / process_count;

    printf(" > Average Waiting Time       : %.2f sec\n", avg_waiting);
    printf(" > Average Turnaround Time    : %.2f sec\n", avg_turnaround);
    printf("===========================================================================\n\n");
}