#include <stdio.h>   // FILE, fopen, putc, ferror, clearerr, fclose, printf

int main(void)
{
    int ret;        // ferror 반환값 저장
    FILE *fp;       // 파일 포인터

    fp = fopen("test.dat", "r");   // test.dat를 읽기 모드로 open

    putc('?', fp);   // 읽기 모드 파일에 쓰기 시도 (에러 발생)

    if (ret = ferror(fp))   // 에러 발생 여부 확인 (에러면 0이 아닌 값)
        printf("ferror() return %d\n", ret);   // 에러 상태 출력

    clearerr(fp);   // 파일의 에러 상태 초기화

    printf("ferror() return %d\n", ferror(fp));   // 에러 상태 다시 확인 (0이어야 정상)

    fclose(fp);     // 파일 닫기
    return 0;       // 정상 종료
}