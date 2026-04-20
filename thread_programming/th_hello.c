#include <stdio.h>      // 표준 입출력 함수 printf 사용
#include <pthread.h>    // pthread 관련 함수 사용
#include <unistd.h>     // sleep 함수 사용

int global;             // 전역 변수 선언 (이 코드에서는 실제로 사용되지 않음)

void *printmsg(void *msg)   // 스레드가 실행할 함수, 문자열 메시지를 인자로 받음
{                           // 함수 시작
    int i=0;                // 반복문용 지역 변수 i를 0으로 초기화

    while (i<5) {           // i가 5보다 작은 동안 총 5번 반복
        printf("%s : %d \n", (char *) msg, i++);   // 전달받은 문자열과 현재 i 값을 출력한 뒤 i 증가
        sleep(1);           // 1초 동안 대기
    }                       // while 반복문 끝

    pthread_exit((void *)pthread_self());   // 현재 스레드 ID를 반환값처럼 넘기고 스레드 종료
}                           // 함수 끝

void main()                 // 메인 함수 시작
{                           // main 시작
    pthread_t   t1, t2, t3; // 스레드 변수 선언, 실제로는 t1과 t2만 사용하고 t3는 사용되지 않음
    void *rval1;            // 첫 번째 스레드의 반환값을 받을 포인터
    void *rval2;            // 두 번째 스레드의 반환값을 받을 포인터
    int mydata;             // 정수 변수 선언, 이 코드에서는 사용되지 않음

    pthread_create(&t1, NULL, printmsg, "Hello");  // t1 스레드 생성, 속성 없음, printmsg 실행, 인자로 "Hello" 전달
    pthread_create(&t2, NULL, printmsg, "World");  // t2 스레드 생성, 속성 없음, printmsg 실행, 인자로 "World" 전달
    //sleep(3)
    //pthread_cancel(t1); 3초 까지 실행되다가 world 만 실행된다.
    //작업이 남았지만, 강제로 cancel 했으므로, join 도 제대로 id 를 반환하지 못한다. 불완전 종료이므로

    pthread_join(t1, (void *)&rval1);   // t1 스레드가 끝날 때까지 기다리고 반환값을 rval1에 저장
    pthread_join(t2, (void *)&rval2);   // t2 스레드가 끝날 때까지 기다리고 반환값을 rval2에 저장

    printf("t1: %lu, t2: %lu \n", t1, t2);   // 생성된 두 스레드의 ID 출력
    printf("rval1: %lu, rval2: %lu \n",      // join으로 받아온 반환값 출력
        (unsigned long)rval1, (unsigned long)rval2);   // void* 값을 unsigned long으로 형변환해서 출력 rval 은 스레드 id 를 출력한다
}                           // main 끝
