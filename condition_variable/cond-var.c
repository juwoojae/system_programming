#include <pthread.h>   // pthread 라이브러리 (스레드, mutex, condition variable)
#include <stdio.h>     // printf
#include <stdlib.h>    // random()

#define NUM_THREADS 3      // 총 스레드 개수
#define TCOUNT 10          // inc_count 스레드가 count를 증가시키는 횟수
#define COUNT_LIMIT 12     // 조건 (count가 12가 되면 signal 발생)

int count = 0;   // 공유 변수 (모든 스레드가 접근)
int thread_ids[3] = {0,1,2};   // 스레드 ID 저장용 배열

// mutex 초기화 (count 보호용)
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

// condition variable 초기화 (count == 12 기다리는 용도)
pthread_cond_t count_threshold_cv = PTHREAD_COND_INITIALIZER;


/*
 * count를 증가시키는 스레드 함수
 * → 조건(count == 12)을 만족시키는 역할
 */
void *inc_count(void *idp)
{
    int i, j;
    double result = 0.0;
    int *my_id = (int *)idp;   // 스레드 ID 받아오기

    for (i = 0; i < TCOUNT; i++) {

        pthread_mutex_lock(&count_mutex);   // mutex lock (임계구역 진입)

        count++;   // 공유 변수 증가

        // count가 12가 되면 기다리는 스레드 깨움
        if (count == COUNT_LIMIT) {
            pthread_cond_signal(&count_threshold_cv);  // signal 전송
            printf("inc_count(): thread %d, count = %d Threshold reached.\n",
                   *my_id, count);
        }
        //count 를 건들지 않아서 race condition 안생김
        printf("inc_count(): thread %d, count = %d, unlocking mutex\n",
               *my_id, count);

        pthread_mutex_unlock(&count_mutex);   // mutex unlock (임계구역 탈출)

        // 일부러 시간 소비 → 스레드 간 경쟁 유도
        for (j = 0; j < 1000; j++)
            result = result + (double)random();
    }

    pthread_exit(NULL);   // 스레드 종료
}


/*
 * count가 특정 값(COUNT_LIMIT)이 될 때까지 기다리는 스레드
 * → condition variable의 wait 사용하는 쪽
 */
void *watch_count(void *idp)
{
    int *my_id = (int *)idp;

    printf("Starting watch_count(): thread %d\n", *my_id);

    pthread_mutex_lock(&count_mutex);   // mutex lock

    // 조건이 만족될 때까지 반복해서 wait (중요: while)
    while (count < COUNT_LIMIT) { //count 는 share 하는 변수임.

        // wait:
        // 1. mutex 자동 unlock
        // 2. sleep (blocked)
        // 3. signal 받으면 깨어남
        // 4. mutex 다시 lock
        pthread_cond_wait(&count_threshold_cv, &count_mutex);

        printf("watch_count(): thread %d Condition signal received.\n", *my_id);
    }

    pthread_mutex_unlock(&count_mutex);   // mutex unlock

    pthread_exit(NULL);   // 스레드 종료
}


/*
 * 메인 함수
 * → 스레드 생성 및 종료 대기
 */
int main(int argc, char *argv[])
{
    int i;
    pthread_t threads[NUM_THREADS];   // 스레드 배열

    // 두 개의 inc_count 스레드 생성 (count 증가 역할)
    pthread_create(&threads[0], NULL, inc_count, (void *)&thread_ids[0]);
    pthread_create(&threads[1], NULL, inc_count, (void *)&thread_ids[1]);

    // 하나의 watch_count 스레드 생성 (조건 기다리는 역할)
    pthread_create(&threads[2], NULL, watch_count, (void *)&thread_ids[2]);

    // 모든 스레드 종료 대기
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Main(): Waited on %d threads. Done.\n", NUM_THREADS);

    pthread_exit(NULL);   // main 종료
}