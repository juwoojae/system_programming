#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];                 // pipe용 파일 디스크립터
    int input, output;
    int n;
    char buf[10];

    pipe(fd);                  // 파이프 생성

    if (fork() == 0) {         // child process
        input = open("input.dat", O_RDONLY);   // 입력 파일 열기

        close(fd[0]);          // 읽기 끝 닫기

        while ((n = read(input, buf, 10)) > 0) // 파일 끝까지 읽기
            write(fd[1], buf, n);              // pipe로 보내기

        close(input);          // 파일 닫기
        close(fd[1]);          // pipe write 닫기 (EOF 전달)
        exit(0);
    }
    else {                     // parent process
        close(fd[1]);          // write 끝 닫기

        output = creat("output.dat", 0666); // 출력 파일 생성

        while ((n = read(fd[0], buf, 10)) > 0) // pipe에서 읽기
            write(output, buf, n);             // 파일에 쓰기

        close(output);
        close(fd[0]);
        wait(NULL);            // 자식 종료 대기
    }

    return 0;
}
