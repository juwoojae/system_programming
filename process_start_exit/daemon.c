#include <stdio.h>      // printf, perror 사용
#include <stdlib.h>     // exit 사용
#include <sys/types.h>  // pid_t 사용
#include <unistd.h>     // fork, getpid, close, setsid, sleep 사용

int main(void)
{
    pid_t   pid;        // 프로세스 ID 저장용 변수
    int i = 10000;      // 자식 프로세스에서 증가시키며 사용할 값

    if ((pid = fork()) < 0) {                    // 자식 프로세스 생성
        perror("fork error");                    // fork 실패 시 에러 출력
    } else if (pid == 0) {   /* child process -> daemon */
        printf("Child: pid=%d\n", getpid());     // 자식 프로세스 PID 출력
        close(0);                                // stdin 닫기
        close(1);                                // stdout 닫기
        close(2);                                // stderr 닫기
        setsid();        // 터미널과 분리, 새로운 세션/프로세스 그룹 생성

        while (1) {                              // 데몬처럼 계속 실행
            printf("C(%d)\n", i);                // 하지만 stdout이 닫혀 있어서 화면에 안 보임
            i++;                                 // 값 증가
            sleep(1);                            // 1초 대기
        }
    } else {             /* parent process */
        printf("Parent: pid=%d\n", getpid());    // 부모 프로세스 PID 출력
        printf("Parent: Child pid=%d\n", pid);   // 생성된 자식 PID 출력

        sleep(1);                                // 잠깐 대기

        printf("Parent : exit\n");               // 부모 종료 메시지
        exit(0);                                 // 부모 프로세스 종료
    }
}
