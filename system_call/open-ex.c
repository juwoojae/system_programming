#include <fcntl.h>   // open() 같은 파일 관련 시스템콜을 쓰기 위한 헤더
#include <stdio.h>   // FILE*, fprintf, perror 등을 쓰기 위한 헤더

int main(int argc, char *argv[])  // 실행 시 인자를 받는 메인 함수
{
    FILE *fpo;    // 라이브러리 레벨 파일 포인터
    int fdo;      // 시스템콜에서 사용하는 파일 디스크립터(fd)

    if(argc != 2) {                  // 인자가 2개가 아니면 (프로그램명 + 파일명)
        perror(argv[0]);             // 에러 메시지 출력
        return 1;                    // 프로그램 종료
    }

    if((fdo = open(argv[1],          // argv[1] 파일을 열기
                   O_RDWR | O_CREAT | O_TRUNC,  // 읽기/쓰기 + 없으면 생성 + 기존 내용 삭제
                   S_IRUSR | S_IWUSR)) == -1) { // 사용자 읽기/쓰기 권한
        perror(argv[1]);             // 파일 열기 실패 시 에러 출력
        return 1;                    // 종료
                   }

    if((fpo = fdopen(fdo, "r+")) == NULL) {  // fd → FILE*로 변환 (읽기/쓰기 모드)
        perror("fdopen");                    // 변환 실패 시 에러 출력
        return 2;                            // 종료
    }

    fprintf(fpo, "Hello, world!\n");  // 파일에 문자열 쓰기 (라이브러리 함수)

    fclose(fpo);   // 파일 닫기 (fd도 같이 닫힘)

    return 0;      // 정상 종료
}
