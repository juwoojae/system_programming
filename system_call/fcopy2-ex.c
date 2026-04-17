#include <stdio.h>        // printf, perror 사용
#include <sys/stat.h>     // 파일 권한 관련 상수
#include <fcntl.h>       // open() 사용 (오타 있음: include가 맞음)
#define BUFFER_SIZE 1024  // 버퍼 크기 정의

int main(int argc, char *argv[])
{
    int fdi, fdo;                     // 입력 파일, 출력 파일 디스크립터
    char buf[BUFFER_SIZE];            // 데이터 임시 저장 버퍼
    ssize_t n;                        // read()가 반환하는 바이트 수

    if(argc != 3) {                   // 인자 개수 체크 (프로그램명 + 2개 파일)
        perror(argv[0]);              // 에러 메시지 출력
        return 1;                     // 종료
    }

    if((fdi = open(argv[1], O_RDONLY)) == -1) {   // 입력 파일을 읽기 전용으로 오픈
        perror(argv[1]);                          // 실패 시 파일 이름 출력
        return 2;                                // 종료
    }

    if((fdo = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL,
                   S_IRUSR | S_IWUSR)) == -1) {   // 출력 파일 생성 (쓰기 전용, 기존 있으면 실패)
        perror(argv[2]);                          // 실패 시 파일 이름 출력
        return 3;                                // 종료
                   }

    while((n = read(fdi, buf, BUFFER_SIZE)) > 0)  // 입력 파일에서 읽기
        write(fdo, buf, n);                       // 읽은 만큼 출력 파일에 쓰기

    close(fdi);   // 입력 파일 닫기
    close(fdo);   // 출력 파일 닫기
    return 0;     // 정상 종료
}
