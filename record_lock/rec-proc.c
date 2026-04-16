#include <stdio.h>      // printf, scanf, perror, FILE, fdopen, fread, fwrite, fseek, fclose
#include <stdlib.h>     // exit
#include <fcntl.h>      // open, O_RDWR, O_CREAT
#include <unistd.h>     // close
#include <sys/stat.h>   // S_IRUSR, S_IWUSR

#define NUM_RECORDS 100   // 최대 레코드 개수

struct record {          // 하나의 계좌 레코드 구조체
    char name[20];       // account owner
    int id;              // account number
    int balance;         // balance
};

void get_new_record(struct record *curr);   // 새 레코드 정보 입력 함수
void display_record(struct record *curr);   // 레코드 출력 함수

int main(int argc, char *argv[])
{
    FILE *fp;                          // stdio용 파일 포인터
    struct record current;             // 현재 읽은 레코드 저장용
    int record_no;                     // 접근할 레코드 번호
    int fd;                            // low-level file descriptor
    long pos;                          // 파일 내 offset 위치
    char yes;                          // 수정 여부 입력 문자

    if (argc < 2) {                    // 파일명이 전달되지 않은 경우
        printf("usage: %s <datafile>\n", argv[0]);   // 사용법 출력
        exit(1);                       // 프로그램 종료
    }

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // argv[1] 파일을 읽기/쓰기 모드로 열고, 없으면 생성
    // 권한은 소유자 읽기/쓰기

    if (fd < 0) {                      // open 실패 시
        perror(argv[1]);               // 에러 메시지 출력
        exit(1);                       // 종료
    }

    if ((fp = fdopen(fd, "r+")) == NULL)   // fd를 FILE* 스트림으로 변환
    {
        perror(argv[1]);               // 에러 메시지 출력
        close(fd);                     // fd 닫기
        exit(2);                       // 종료
    }

    printf("enter record number: ");   // 레코드 번호 입력 요청
    scanf("%d", &record_no);           // 레코드 번호 입력
    getchar();                         // 입력 버퍼의 개행 제거

    while (record_no >= 0 && record_no < NUM_RECORDS) {   // 0~99 범위 동안 반복
        pos = record_no * sizeof(struct record);          // 해당 레코드의 파일 위치 계산

        fseek(fp, pos, SEEK_SET);                         // 해당 위치로 이동

        if (fread(&current, sizeof(struct record), 1, fp) != 1) {
            // 읽을 데이터가 없으면 빈 레코드처럼 초기화
            current.id = 0;                              // 기본 id
            current.balance = 0;                         // 기본 balance
            current.name[0] = '\0';                      // 빈 문자열
        }

        display_record(&current);                        // 현재 레코드 출력

        printf("update records? yes = y\n");             // 수정 여부 질문
        scanf("%c", &yes);                               // 문자 입력
        getchar();                                       // 개행 제거

        if (yes == 'y') {                                // y 입력 시 수정 수행
            get_new_record(&current);                    // 새 값 입력받기

            fseek(fp, pos, SEEK_SET);                    // 다시 같은 위치로 이동
            fwrite(&current, sizeof(struct record), 1, fp);   // 수정된 내용 저장

            printf("update done\n");                     // 완료 메시지
        }

        printf("enter record number: ");                 // 다음 레코드 번호 입력
        scanf("%d", &record_no);                         // 입력
        getchar();                                       // 개행 제거
    }

    fclose(fp);                                          // 파일 닫기
    return 0;                                            // 정상 종료
}

void get_new_record(struct record *curr)
{
    printf("> id? ");                    // id 입력 요청
    scanf("%d", &curr->id);             // id 입력

    printf("> name? ");                 // name 입력 요청
    scanf("%19s", curr->name);          // 최대 19글자까지 입력, 마지막은 '\0'

    printf("> balance? ");              // balance 입력 요청
    scanf("%d", &curr->balance);        // balance 입력
    getchar();                          // 개행 제거
}

void display_record(struct record *curr)
{
    printf("\n");                       // 줄바꿈
    printf("id: %d\n", curr->id);       // id 출력
    printf("name: %s\n", curr->name);   // 이름 출력
    printf("balance: %d\n", curr->balance);   // 잔액 출력
    printf("\n");                       // 줄바꿈
}