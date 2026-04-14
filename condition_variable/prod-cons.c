#include <stdio.h>        // printf
#include <pthread.h>      // pthread 라이브러리

void *producer(void*);    // producer 함수 선언
void *consumer(void*);    // consumer 함수 선언

#define MAX_BUF 100       // 버퍼 크기

// 공유 변수
int buffer[MAX_BUF];      // 데이터 저장 버퍼
int count = 0;            // 현재 버퍼에 들어있는 데이터 개수
int in = -1, out = -1;    // 넣는 위치 / 꺼내는 위치

// mutex (공유자원 보호)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// condition variable
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER; // 공간 있음
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;  // 데이터 있음

int main(void)
{
    int i;
    pthread_t threads[2];   // producer 0, consumer 1

    // 스레드 생성
    pthread_create(&threads[0], NULL, producer, NULL);
    pthread_create(&threads[1], NULL, consumer, NULL);

    // 종료 대기
    for (i=0; i< 2; i++)
        pthread_join(threads[i], NULL);

    return 0;
}

/*
 * producer: 데이터 생성해서 buffer에 넣음
 */
void *producer (void *v)
{
    int i;

    for (i =0; i < 1000; i++) {

        pthread_mutex_lock(&mutex);   // lock

        // 버퍼가 가득 차면 기다림
        if (count == MAX_BUF)
            pthread_cond_wait(&buffer_has_space, &mutex);

        // 가득차지 않았다면
        in = (in + 1) % MAX_BUF;   // 다음 위치 계산
        buffer[in] = i;            // 데이터 넣기
        count++;                  // 개수 증가

        // 데이터 생겼으니 consumer 깨움
        pthread_cond_signal(&buffer_has_data);

        pthread_mutex_unlock(&mutex);   // unlock
    }
}

/*
 * consumer: buffer에서 데이터 꺼냄
 */
void *consumer (void *v)
{
    int i, data;

    for (i =0; i < 1000; i++) {

        pthread_mutex_lock(&mutex);   // lock

        // 버퍼가 비어있으면 기다림
        if (count == 0)
            pthread_cond_wait(&buffer_has_data, &mutex);

        out = (out + 1) % MAX_BUF;   // 다음 위치 계산
        data = buffer[out];          // 데이터 꺼냄
        count--;                     // 개수 감소

        // 공간 생겼으니 producer 깨움
        pthread_cond_signal(&buffer_has_space);

        pthread_mutex_unlock(&mutex);   // unlock

        printf("data = %d\n", data);   // 출력
    }
}