#include <stdio.h>      // FILE, fopen, fgets, fputs, fclose, setvbuf

#define BUFFER_SIZE 100 // 버퍼 크기 정의

int main(int argc, char *argv[])
{
    char ubuf[BUFFER_SIZE], line[BUFFER_SIZE];
    // ubuf : 파일 스트림 버퍼로 사용할 배열
    // line : 파일에서 한 줄씩 읽어올 문자열 버퍼

    FILE *fpin, *fpout; // 입력 파일 스트림, 출력 파일 스트림

    // 인자 개수 체크 (프로그램 + 입력파일 + 출력파일)
    if (argc != 3) {
        perror(argv[0]);   // 프로그램 이름 기준 에러 출력
        return 1;          // 종료
    }

    // 입력 파일 열기 (읽기 모드)
    if ((fpin = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);   // 파일 이름 기준 에러 출력
        return 2;
    }

    // 출력 파일 열기 (append 모드)
    if ((fpout = fopen(argv[2], "a")) == NULL) {
        perror(argv[2]);   // 파일 이름 기준 에러 출력
        return 3;
    }

    // 입력 파일 스트림 버퍼 설정 (line buffering)
    if (setvbuf(fpin, ubuf, _IOLBF, BUFFER_SIZE) != 0) {
        // ubuf를 버퍼로 사용, 줄 단위로 flush됨
        perror("setvbuf(fpin)");
        return 4;
    }

    // 출력 파일 스트림 버퍼 설정 (line buffering)
    if (setvbuf(fpout, ubuf, _IOLBF, BUFFER_SIZE) != 0) {
        perror("setvbuf(fpout)");
        return 5;
    }

    // 입력 파일에서 한 줄씩 읽어서 출력 파일에 씀
    while (fgets(line, BUFFER_SIZE, fpin) != NULL)
        // fgets : 한 줄 단위로 읽음 (개행 포함)
            fputs(line, fpout); // 읽은 문자열을 그대로 출력 파일에 기록

    fclose(fpin);   // 입력 파일 닫기
    fclose(fpout);  // 출력 파일 닫기

    return 0;       // 정상 종료
}
