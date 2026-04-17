#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;          // 파일 포인터
    char buf[256];     // 파일에서 읽은 문자열을 저장할 버퍼
    int rspn;          // fseek 반환값 저장
    long pos;          // 현재 파일 offset 저장

    if((fp = fopen(argv[1], "r")) == NULL) {   // argv[1] 파일을 읽기 모드로 open
        perror(argv[1]);                       // 파일 open 실패 시 에러 출력
        return 1;                              // 프로그램 종료
    }

    rspn = fseek(fp, 8L, SEEK_SET);            // 파일 시작(SEEK_SET) 기준으로 8 byte 위치로 offset 이동
    pos = ftell(fp);                           // 현재 파일 offset 값을 pos에 저장

    fgets(buf, 256, fp);                       // 현재 offset 위치부터 한 줄 읽어서 buf에 저장
    printf("Position : %ld\n", pos);            // 현재 offset 위치 출력
    printf("%s\n", buf);                       // 읽은 문자열 출력

    rewind(fp);                                // 파일 offset을 파일 시작(0)으로 이동
    pos = ftell(fp);                           // 현재 offset 다시 확인

    fgets(buf, 256, fp);                       // 파일 처음부터 한 줄 읽기
    fclose(fp);                                // 파일 닫기
    printf("Position : %ld\n", pos);            // 현재 offset 위치 출력
    printf("%s\n", buf);                       // 읽은 문자열 출력
    return 0;                                  // 프로그램 정상 종료
}
