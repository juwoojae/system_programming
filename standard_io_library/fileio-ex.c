#include <stdio.h>      // FILE, fopen, getc, putc, fclose
#include <stdlib.h>     // exit

// argc = 3
// argv[0] = "./fileio-ex"
// argv[1] = "firstFile"
// argv[2] = "secondFile"

int main(int argc, char *argv[])
{
    int c;                          // 한 문자씩 저장할 변수
    FILE *fpin, *fpout;             // 입력 파일 스트림, 출력 파일 스트림

    // 인자 개수가 3개가 아니면 에러 처리
    if (argc != 3) {
        perror(argv[0]);            // 프로그램 이름 기준 에러 출력
        exit(1);                    // 종료 코드 1
    }

    // 입력 파일 열기 (읽기 모드)
    if ((fpin = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);            // 파일 이름 기준 에러 출력
        exit(2);                    // 종료 코드 2
    }

    // 출력 파일 열기 (append 모드)
    if ((fpout = fopen(argv[2], "a")) == NULL) {
        perror(argv[2]);            // 파일 이름 기준 에러 출력
        exit(3);                    // 종료 코드 3
    }

    // 입력 파일 버퍼링 제거 (unbuffered)
    setbuf(fpin, NULL);

    // 출력 파일 버퍼링 제거 (unbuffered)
    setbuf(fpout, NULL);

    // 입력 파일에서 한 글자씩 읽어서 출력 파일에 씀
    while ((c = getc(fpin)) != EOF)   // EOF까지 반복
        putc(c, fpout);               // 읽은 문자 그대로 출력 파일에 기록

    fclose(fpin);   // 입력 파일 닫기
    fclose(fpout);  // 출력 파일 닫기

    exit(0);        // 정상 종료
}
