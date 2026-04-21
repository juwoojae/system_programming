#include <pthread.h>   // pthread 라이브러리 사용 (스레드, mutex 함수)
#include <stdio.h>     // printf 사용
#include <stdlib.h>    // malloc, free 사용
#include <stdint.h>

typedef struct {                 // 여러 스레드가 공유할 데이터를 묶는 구조체 정의 시작
    double *a;                   // 첫 번째 벡터의 시작 주소
    double *b;                   // 두 번째 벡터의 시작 주소
    double sum;                  // 최종 dot product 결과를 저장할 공유 변수
    int veclen;                  // 각 스레드가 처리할 벡터 길이
} DOTDATA;                       // 구조체 별칭을 DOTDATA로 정의

#define NUMTHRDS 4               // 사용할 스레드 개수 4개
#define VECLEN 100               // 각 스레드가 맡을 원소 개수 100개

DOTDATA dotstr;                  // 모든 스레드가 공유할 전역 구조체 변수
pthread_t callThd[NUMTHRDS];     // 생성할 스레드들의 ID를 저장할 배열
pthread_mutex_t mutexsum;        // 공유 변수 sum 보호용 mutex

void *dotprod(void *arg)         // 각 스레드가 실행할 함수
{
    int i, start, end, offset, len;   // 반복 변수, 시작 인덱스, 끝 인덱스, 스레드 번호, 길이
    double mysum, *x, *y;             // 각 스레드의 부분합, a 벡터 포인터, b 벡터 포인터

    offset = (int)(intptr_t)arg;      // 전달받은 스레드 번호를 정수로 변환
    len = dotstr.veclen;              // 한 스레드가 처리할 길이 가져오기
    start = offset * len;             // 현재 스레드가 시작할 인덱스 계산
    end = start + len;                // 현재 스레드가 끝낼 인덱스 계산
    x = dotstr.a;                     // 첫 번째 벡터 주소를 x에 저장
    y = dotstr.b;                     // 두 번째 벡터 주소를 y에 저장

    mysum = 0;                        // 현재 스레드의 부분합을 0으로 초기화
    for (i = start; i < end; i++) {   // 자기 담당 구간만 반복 start = 100, end = 200
        mysum += (x[i] * y[i]);       // A[i] * B[i]를 부분합에 누적
    }
    //임계구역 시작
    pthread_mutex_lock(&mutexsum);    // 공유 변수 sum 수정 전에 mutex 잠금
    dotstr.sum += mysum;              // 현재 스레드의 부분합을 전체 합에 더함
    pthread_mutex_unlock(&mutexsum);  // sum 수정이 끝났으므로 mutex 해제

    pthread_exit((void *)0);          // 스레드 종료
}

int main(int argc, char *argv[])      // 메인 함수 시작
{
    int i;                            // 반복문 변수
    double *a, *b;                    // 두 벡터의 동적 배열 포인터
    void *status;                       // pthread_join에서 상태값 받을 변수

    a = (double *)malloc(NUMTHRDS * VECLEN * sizeof(double));   // 전체 벡터 a를 위한 메모리 할당
    b = (double *)malloc(NUMTHRDS * VECLEN * sizeof(double));   // 전체 벡터 b를 위한 메모리 할당

    for (i = 0; i < VECLEN * NUMTHRDS; i++)   // 전체 원소 개수만큼 반복
        b[i] = a[i] = 1;                      // 테스트를 쉽게 하려고 a, b의 모든 값을 1로 설정

    dotstr.veclen = VECLEN;           // 구조체에 각 스레드 담당 길이 저장
    dotstr.a = a;                     // 구조체에 a 벡터 주소 저장
    dotstr.b = b;                     // 구조체에 b 벡터 주소 저장
    dotstr.sum = 0;                   // 최종 합 초기화

    pthread_mutex_init(&mutexsum, NULL);   // mutex 초기화

    for (i = 0; i < NUMTHRDS; i++) {       // 스레드 개수만큼 반복
        pthread_create(&callThd[i], NULL, dotprod, (void *)(intptr_t)i);  // i번 스레드 생성, dotprod 실행
    }

    for (i = 0; i < NUMTHRDS; i++) {       // 모든 스레드에 대해 반복
        pthread_join(callThd[i], &status);             // 각 스레드 종료까지 대기
    }

    printf("Sum = %f\n", dotstr.sum);      // 최종 dot product 결과 출력

    free(a);                               // 동적 할당한 a 메모리 해제
    free(b);                               // 동적 할당한 b 메모리 해제

    pthread_mutex_destroy(&mutexsum);      // mutex 자원 해제
    pthread_exit(NULL);                    // 메인 스레드 종료
}

