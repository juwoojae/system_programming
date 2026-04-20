#include <sys/types.h>   // pid_t 같은 프로세스 관련 타입 정의
#include <unistd.h>      // fork(), getpid(), getppid(), getpgrp(), getpgid() 선언
#include <stdio.h>       // printf() 선언

int main()               // 프로그램 시작점
{
    int pid;             // fork()의 반환값을 저장할 변수
    printf("Original process : pid = %d, ppid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));   // fork 전 원래 프로세스의 PID, 부모 PID, 프로세스 그룹 정보 출력

    pid = fork();        // 현재 프로세스를 복제하여 자식 프로세스 생성
    if(pid != 0) {       // 부모 프로세스인 경우 (자식 PID가 반환됨, 0이 아님)
        printf("Parent process : pid = %d, ppid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));   // 부모 프로세스 정보 출력
    }
    else {               // 자식 프로세스인 경우 (fork() 반환값이 0)
        printf("Child process : pid = %d, ppid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));   // 자식 프로세스 정보 출력
    }
    return 0;            // 프로그램 정상 종료
}
