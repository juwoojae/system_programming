#include <stdio.h>
#include <sys/stat.h>    // 파일 권한 관련 상수 (S_IRUSR 등)
#include <unistd.h>      // write(), lseek() 같은 system call

int main(void)
{
    int fd;                              // 파일 디스크립터
    char buf1[] = "Test1 data";          // 첫 번째 쓸 데이터
    char buf2[] = "Test2 data";          // 두 번째 쓸 데이터

    if ((fd = creat("test.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {   // 파일 생성 (rw-r--r--)
        printf("creat error");           // 생성 실패 시 에러 출력
        return 1;                        // 프로그램 종료
    }

    write(fd, buf1, 10);                 // "Test1 data" 중 10바이트 쓰기

    if (lseek(fd, 6L, SEEK_SET) == -1) { // 파일 위치를 6번째 바이트로 이동
        printf("lseek error");           // 실패 시 에러 출력
        return 2;                        // 프로그램 종료
    }

    write(fd, buf2, 10);                 // 6번째 위치부터 "Test2 data" 덮어쓰기

    return 0;                            // 정상 종료
}
