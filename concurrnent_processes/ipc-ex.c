#include <stdio.h>      // perror 함수 사용
#include <stdlib.h>     // exit 함수 사용
#include <unistd.h>     // pipe, fork, read, write, close 함수 사용
#include <fcntl.h>      // open, creat 함수 사용
#include <sys/wait.h>   // wait 함수 사용
#include <ctype.h>      // toupper 함수 사용

int main(int argc, char *argv[]) {   // 프로그램 시작, 명령행 인자 사용
    int fd1[2], fd2[2];              // fd1: parent -> child1 파이프, fd2: child1 -> child2 파이프
    int input;                       // 입력 파일 디스크립터
    int n;                           // read/write 한 바이트 수 저장
    char buf[10];                    // 데이터를 10바이트씩 임시 저장할 버퍼

    if (argc != 2) {                 // 실행 시 파일 이름이 정확히 1개 들어왔는지 확인
        perror(argv[0]);             // 아니면 에러 메시지 출력
        return 1;                    // 프로그램 종료
    }

    // 1. 파일 open 하기
    if ((input = open(argv[1], O_RDONLY)) == -1) {   // 입력 파일을 읽기 전용으로 open
        perror(argv[1]);                             // open 실패 시 파일명과 함께 에러 출력
        return 2;                                    // 프로그램 종료
    }

    // 2. pipe 2개 생성
    pipe(fd1);                    // 첫 번째 파이프 생성: parent가 쓰고 child1이 읽음
    pipe(fd2);                    // 두 번째 파이프 생성: child1이 쓰고 child2가 읽음

    pid_t pid1 = fork();          // 첫 번째 fork: child1 생성

    if (pid1 == 0) {              // child1 프로세스인 경우
        pid_t pid2 = fork();      // child1이 다시 fork해서 child2 생성

        if (pid2 == 0) {          // child2 프로세스인 경우: pipe2에서 읽어서 파일에 씀
            close(fd1[0]);        // fd1의 읽기 끝 닫기 (사용 안 함)
            close(fd1[1]);        // fd1의 쓰기 끝 닫기 (사용 안 함)
            close(fd2[1]);        // fd2는 읽기만 하므로 쓰기 끝 닫기
            close(input);         // 입력 파일 안 쓰므로 닫기

            int output = creat("output.dat", 0666);  // 결과를 저장할 output.dat 파일 생성
            while ((n = read(fd2[0], buf, 10)) > 0)  // pipe2에서 10바이트씩 읽기
                write(output, buf, n);               // 읽은 데이터를 output.dat에 그대로 쓰기
            close(output);                           // 출력 파일 닫기
            close(fd2[0]);                           // pipe2 읽기 끝 닫기
            exit(0);                                 // child2 종료

        } else {                    // child1 프로세스인 경우: pipe1에서 읽어서 대문자로 바꾼 뒤 pipe2로 보냄
            close(fd1[1]);          // fd1은 읽기만 하므로 쓰기 끝 닫기
            close(fd2[0]);          // fd2는 쓰기만 하므로 읽기 끝 닫기
            close(input);           // 입력 파일 안 쓰므로 닫기

            while ((n = read(fd1[0], buf, 10)) > 0) {   // pipe1에서 10바이트씩 읽기
                for (int i = 0; i < n; i++)             // 읽은 바이트 수만큼 반복
                    buf[i] = toupper(buf[i]);           // 각 문자를 대문자로 변환
                write(fd2[1], buf, n);                  // 변환한 데이터를 pipe2로 쓰기
            }
            close(fd1[0]);          // pipe1 읽기 끝 닫기
            close(fd2[1]);          // pipe2 쓰기 끝 닫기 -> child2에게 EOF 전달
            wait(NULL);             // child2가 끝날 때까지 대기
            exit(0);                // child1 종료
        }

    } else {                        // parent 프로세스인 경우: 파일 읽어서 pipe1로 보냄
        close(fd1[0]);              // fd1은 쓰기만 하므로 읽기 끝 닫기
        close(fd2[0]);              // fd2 사용 안 하므로 읽기 끝 닫기
        close(fd2[1]);              // fd2 사용 안 하므로 쓰기 끝 닫기

        while ((n = read(input, buf, 10)) > 0)   // 입력 파일에서 10바이트씩 읽기
            write(fd1[1], buf, n);               // 읽은 데이터를 pipe1로 보내기
        close(input);              // 입력 파일 닫기
        close(fd1[1]);             // pipe1 쓰기 끝 닫기 -> child1에게 EOF 전달
        wait(NULL);                // child1이 끝날 때까지 대기
    }

    return 0;                      // parent 종료
}