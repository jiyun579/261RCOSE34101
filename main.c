#include "header.h"

int main() {

    srand((unsigned int)time(NULL));

    int process_count = 0;

    printf("\nEnter the number of processes you want to create (Recommended: 5 to 10): ");
    scanf("%d", &process_count);

    Process* primary_processes = (Process*)malloc(sizeof(Process) * process_count);

    // 프로세스 생성 및 출력
    Create_Process(primary_processes, process_count);
    Print_Process_List(primary_processes, process_count);

    // 알고리즘 설정
    Config();

    // 5. 큐(Queue) 역시 글로벌이 아닌 main의 지역 포인터 배열로 선언 가능!
    Process** ready_queue = (Process**)malloc(sizeof(Process*) * process_count);
    int ready_count = 0;

    // ----------------------------------------------------
    // TODO: 여기에 Schedule(primary_processes, ready_queue, &ready_count, process_count...) 
    // 형태로 매개변수를 넘겨주며 시뮬레이션을 돌리면 됩니다.
    // ----------------------------------------------------

    // 6. 프로그램 종료 전 동적 할당한 메모리 해제 (C언어 필수 매너)
    free(primary_processes);
    free(ready_queue);

    return 0;
}