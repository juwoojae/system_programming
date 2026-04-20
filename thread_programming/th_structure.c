#include <pthread.h>   // pthread 라이브러리
#include <stdio.h>     // printf
#include <malloc.h>    // 메모리 관련 헤더(여기선 사실 없어도 됨)
#include <string.h>    // strcpy 사용

#define NUM_THREADS 3   // 생성할 스레드 개수

struct thread_data {    // 스레드에 넘길 데이터 구조체
    int thread_id;      // 스레드 번호
    int sum;            // 누적합 값
    char *message;      // 메시지 문자열
};

struct thread_data thread_data_array[NUM_THREADS];   // 각 스레드별 데이터 저장 배열
pthread_t threads[NUM_THREADS];                      // 스레드 ID 저장 배열

void *printHello(void *arg)   // 스레드 함수
{
    struct thread_data *my_data;   // 전달받은 구조체 포인터
    int taskid, sum;               // 구조체에서 꺼낸 값 저장
    char *hello_msg;               // 구조체에서 꺼낸 메시지 저장

    my_data = (struct thread_data *) arg;   // void* 인자를 구조체 포인터로 형변환
    taskid = my_data->thread_id;            // thread_id 값 가져오기
    sum = my_data->sum;                     // sum 값 가져오기
    hello_msg = my_data->message;           // message 값 가져오기

    printf("taskid = %d\n", taskid);        // 스레드 번호 출력
    printf("sum = %d\n", sum);              // sum 값 출력
    printf("message = %s\n", hello_msg);    // 메시지 출력

    pthread_exit(NULL);   // 스레드 종료
}

int main(int argc, char *argv[])
{
    int rc, i, sum = 0;         // rc: create 결과, i: 반복문 변수, sum: 누적합
    void *res;                  // join 받을 반환값
    char messages[3][1024];     // 문자열 3개 저장할 배열

    strcpy(messages[0], "hello");                 // 첫 번째 메시지
    strcpy(messages[1], "system programming");   // 두 번째 메시지
    strcpy(messages[2], "world");                // 세 번째 메시지

    for (i = 0; i < 3; i++) {   // 3개 스레드 생성
        sum += i;                                   // 누적합 계산
        thread_data_array[i].thread_id = i;         // 구조체에 id 저장
        thread_data_array[i].sum = sum;             // 구조체에 sum 저장
        thread_data_array[i].message = messages[i]; // 구조체에 메시지 저장

        rc = pthread_create(&threads[i], NULL, printHello,
                            (void *)&thread_data_array[i]);
        // 각 스레드에 자기 구조체 주소 전달
    }

    for (i = 0; i < 3; i++) {   // 생성한 스레드들 종료 대기
        pthread_join(threads[i], &res);
    }

    return 0;   // 메인 종료
}
