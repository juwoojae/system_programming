#include <sys/types.h>   // 시스템 데이터 타입 정의
#include <sys/stat.h>    // 파일 상태 관련 헤더
#include <unistd.h>      // link() 함수 선언
#include <stdio.h>       // perror(), printf()

int main(int argc, char *argv[])
{
    if(argc != 3) {                      // 프로그램명 포함 총 3개 인자 필요
        perror("argument error");        // 인자 개수 오류 출력
        return 1;                        // 종료
    }

    if (link(argv[1], argv[2]) < 0) {    // argv[1] 파일(링크) 에 대해 argv[2]라는 하드 링크 생성
        perror("link fail");             // 실패 원인 출력
        return 2;                        // 종료
    }
}
