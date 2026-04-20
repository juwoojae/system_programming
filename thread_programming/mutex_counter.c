#include <stdio.h>      // printf 함수 사용
#include <stdlib.h>     // 표준 라이브러리 함수 사용 가능
#include <pthread.h>    // pthread, mutex 관련 함수 사용
#include <unistd.h>     // sleep 함수 사용

void *Incrementer(void *arg);   // 증가 스레드 함수 선언
void *Decrementer(void *arg);   // 감소 스레드 함수 선언

int counter = 0;   // 여러 스레드가 함께 접근하는 공유 변수

pthread_mutex_t mVar;   // 공유 변수 보호용 mutex 변수

int main()
{
    pthread_t ptid, ctid;   // 스레드 ID 저장 변수

    pthread_mutex_init(&mVar, NULL);   // mutex 초기화

    pthread_create(&ptid, NULL, Incrementer, NULL);   // 증가 스레드 생성 속성X, 파라미터 X
    pthread_create(&ctid, NULL, Decrementer, NULL);   // 감소 스레드 생성

    pthread_join(ptid, NULL);   // 증가 스레드 종료까지 대기
    pthread_join(ctid, NULL);   // 감소 스레드 종료까지 대기

    pthread_mutex_destroy(&mVar);   // mutex 자원 해제

    return 0;   // 프로그램 종료
}

void *Incrementer(void *arg)
{
    for (;;)   // 무한 반복
    {
        pthread_mutex_lock(&mVar);   // mutex 잠금 -> 임계구역 진입

        counter++;   // 공유 변수 1 증가
        printf("Inc : %d \n", counter);   // 증가 후 값 출력

        pthread_mutex_unlock(&mVar);   // mutex 해제 -> 임계구역 종료

        sleep(1);   // 1초 쉬기 (출력 너무 빠른 것 방지)
    }

    return NULL;   // 사실상 도달하지 않음
}

void *Decrementer(void *arg)
{
    for (;;)   // 무한 반복
    {
        pthread_mutex_lock(&mVar);   // mutex 잠금 -> 임계구역 진입

        counter--;   // 공유 변수 1 감소
        printf("Dec : %d \n", counter);   // 감소 후 값 출력

        pthread_mutex_unlock(&mVar);   // mutex 해제 -> 임계구역 종료

        sleep(1);   // 1초 쉬기
    }

    return NULL;   // 사실상 도달하지 않음
}
