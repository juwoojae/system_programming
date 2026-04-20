#include <stdio.h>      // printf 사용

void greet();           // 함수 선언 (Hello 출력 함수)
void caller(void (*myFunction)(void));  // 함수 포인터를 매개변수로 받는 함수

int main()
{
    void (*sayHello)(void);  // 함수 포인터 선언 (void 반환, 매개변수 없음)

    printf("Calling greet() directly.\n");
    greet();                 // 1️⃣ 직접 함수 호출

    printf("\n");

    printf("Calling greet() via a pointer.\n");
    sayHello = greet;        // 2️⃣ 함수 주소를 포인터에 저장
    sayHello();              // 포인터를 통해 함수 호출
    printf("\n");

    printf("Calling greet() via another function.\n");
    caller(greet);           // 3️⃣ 함수 포인터를 다른 함수에 전달
}

void greet() {
    printf("Hello, world!\n");   // 실제 실행되는 함수
}

void caller(void (*myFunction)(void)) {  // 함수 포인터를 매개변수로 받음
    myFunction();                        // 전달받은 함수 실행
}
