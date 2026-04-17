#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;                     // 파일 포인터
    char buf[256];                // 문자열 버퍼 (이 코드에서는 사용되지 않음)
    int num, Nnum;                // num : 입력받을 정수, Nnum : 파일에서 읽어올 정수
    char str[30], Nstr[30];       // str : 입력받을 문자열, Nstr : 파일에서 읽어올 문자열

    scanf("%d %s", &num, str);    // 콘솔에서 정수와 문자열 입력

    if((fp = fopen("test.dat", "w")) == NULL) {    // test.dat 파일을 쓰기 모드로 open
        perror("test.dat");                        // 파일 open 실패 시 에러 출력
        return 1;                                  // 프로그램 종료
    }

    fprintf(fp, "%d %s\n", num, str);               // 입력받은 정수와 문자열을 파일에 저장

    if((fp = freopen("test.dat", "r", fp)) == NULL) {  // 같은 파일을 읽기 모드로 다시 open
        perror("test.dat");                            // 파일 open 실패 시 에러 출력
        return 1;                                      // 프로그램 종료
    }

    fscanf(fp, "%d %s\n", &Nnum, Nstr);             // 파일에서 정수와 문자열 읽기
    printf("%d %s\n", Nnum, Nstr);                  // 읽은 데이터를 콘솔에 출력

    fclose(fp);                                     // 파일 닫기
    return 0;                                       // 정상 종료
}
