#include <stdio.h>      // 표준 입출력 (printf, stdout)
#include <sys/stat.h>   // 파일 권한
#include <fcntl.h>      // open()

int main()
{
    int backup_des, stdout_des, ofdes;   // backup_des: stdout 백업, stdout_des: stdout fd, ofdes: 파일 fd

    stdout_des = fileno(stdout);         // stdout의 파일 디스크립터 얻기 (보통 1)
    backup_des = dup(stdout_des);        // stdout을 복제해서 백업 (복구용)

    printf("Hello, world! (1)\n");       // 콘솔에 출력됨

    ofdes = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC,   // test.txt를 쓰기 모드로 생성/초기화
                 S_IRUSR|S_IWUSR);       // 파일 권한: 읽기/쓰기 (owner)

    dup2(ofdes, stdout_des);             // stdout을 test.txt로 리다이렉션
    printf("Hello, world! (2)\n");       // 파일(test.txt)에 출력됨

    dup2(backup_des, stdout_des);        // stdout을 원래 콘솔로 복구
    printf("Hello, world! (3)\n");       // 다시 콘솔에 출력됨

    close(ofdes);                        // 파일 디스크립터 닫기
}
