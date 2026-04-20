#include <pthread.h>   // pthread 라이브러리
#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <unistd.h>    // sleep
#include <stdint.h>    // intptr_t

void *threadFunc(void *arg)   // 스레드 함수
{
    int count = 0;   // 카운터 변수

    printf("new thread started ....\n");   // 스레드 시작 메시지

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    // 취소 가능 상태로 설정

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    // deferred 방식 → cancellation point에서만 종료됨

    while (1)   // 무한 루프
    {
        printf("count = %d\n", count++);   // 값 출력
        sleep(1);   // 1초 대기

        pthread_testcancel();
        // 여기서 취소 요청 확인 → 있으면 종료됨

        if (count == 10)
            break;
        // 테스트용: 10되면 정상 종료
    }

    pthread_exit((void *)(intptr_t)count);
    // int를 바로 void*로 바꾸지 말고 intptr_t 거쳐서 안전하게 변환
}

int main(int argc, char *argv[])
{
    pthread_t tid;   // 스레드 ID
    int retval;      // 함수 반환값
    void *res;       // 스레드 반환값

    retval = pthread_create(&tid, NULL, threadFunc, NULL);
    // 스레드 생성

    if (retval != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    sleep(5);
    // 메인 5초 대기 (thread 실행 중)

    retval = pthread_cancel(tid);
    // 스레드 취소 요청

    if (retval != 0)
    {
        perror("pthread_cancel");
        exit(EXIT_FAILURE);
    }

    retval = pthread_join(tid, &res);
    // 스레드 종료 기다림 + 결과 받기

    if (retval != 0)
    {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    if (res == PTHREAD_CANCELED)
        printf("thread canceled\n");
    // 취소된 경우
    else
        printf("thread is normal exit retval = %ld\n", (long)(intptr_t)res);
    // 정상 종료된 경우

    return 0;
}
