#include <stdio.h>

int main()
{
    int stat = 0;                  // EOF 상태 저장 변수
    FILE *fp;                     // 파일 포인터
    char buf[256];                // 파일에서 읽어올 문자열 버퍼

    fp = fopen("test.dat", "r");  // test.dat 파일을 읽기 모드로 open
    while(!stat)                  // EOF가 발생할 때까지 반복
        if(fgets(buf, 256, fp))   // 파일에서 한 줄 읽기 (성공하면 buf에 저장)
            printf("%s\n", buf);  // 읽은 문자열 출력
        else
            stat = feof(fp);      // 파일 끝에 도달했는지 확인 후 stat에 저장
    printf("feof returned %d\n", stat);  // feof 반환값 출력
    fclose(fp);                   // 파일 닫기
    return 0;                     // 프로그램 정상 종료
}
