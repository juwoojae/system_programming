#include <stdio.h>      // printf, scanf, fgets
#include <stdlib.h>     // exit
#include <fcntl.h>      // open, fcntl, struct flock, F_RDLCK, F_WRLCK, F_UNLCK
#include <unistd.h>     // read, write, lseek, close
#include <sys/stat.h>   // S_IRUSR, S_IWUSR

#define TRUE  1         // 참
#define FALSE 0         // 거짓

#define NUM_RECORDS 100 // 최대 레코드 개수

struct record {
    char name[20];      // account owner
    int id;             // account number
    int balance;        // balance
};

int reclock(int fd, int recno, int len, int type);   // 레코드 락 함수
void display_record(struct record *curr);            // 레코드 출력 함수

int main(void)
{
    struct record current;              // 현재 읽거나 쓸 레코드
    int record_no;                      // 계좌 번호(레코드 번호)
    int fd, pos, n;                     // 파일 디스크립터, 위치, read/write 결과
    char operation;                     // 작업 종류(c/r/d/q)
    int amount;                         // 입금 금액
    char buffer[100];                   // 입력 버퍼 비우기용
    int quit = FALSE;                   // 종료 여부

    fd = open("./account", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // account 파일을 읽기/쓰기 모드로 열고, 없으면 생성

    if (fd < 0) {                       // 파일 열기 실패 시
        perror("open");
        exit(1);
    }

    while (1) {                         // 무한 반복
        printf("enter account number (0-99): ");
        scanf("%d", &record_no);        // 레코드 번호 입력
        fgets(buffer, sizeof(buffer), stdin);   // 남은 개행 제거

        if (record_no < 0 || record_no >= NUM_RECORDS)   // 범위 벗어나면 종료 (탈출 조건)
            break;

        printf("enter operation name (c/r/d/q): ");
        scanf("%c", &operation);        // 작업 문자 입력
        fgets(buffer, sizeof(buffer), stdin);   // 남은 개행 제거


        /**
         * c/r/d/q 에 따라서 분기
         */
        switch (operation) {

        case 'c':   // create
            reclock(fd, record_no, sizeof(struct record), F_WRLCK);
            // 해당 레코드에 write lock (베타락) 획득

            pos = record_no * sizeof(struct record);   // record 위치 계산
            lseek(fd, pos, SEEK_SET);                  // 해당 위치로 이동

            printf("> id ? ");
            scanf("%d", &current.id);                  // id 입력
            fgets(buffer, sizeof(buffer), stdin);      // 개행 제거

            printf("> name ? ");
            scanf("%19s", current.name);               // 이름 입력
            fgets(buffer, sizeof(buffer), stdin);      // 개행 제거

            current.balance = 0;                       // 새 계좌 잔액 초기화

            n = write(fd, &current, sizeof(struct record));   // 파일에 저장
            if (n != sizeof(struct record)) {                 // 쓰기 실패 체크
                perror("write");
            }

            display_record(&current);                  // 저장한 내용 출력

            reclock(fd, record_no, sizeof(struct record), F_UNLCK);
            // lock 해제
            break;

        case 'r':   // inquiry
            reclock(fd, record_no, sizeof(struct record), F_RDLCK);
            // 해당 레코드에 read lock 획득

            pos = record_no * sizeof(struct record);   // 위치 계산
            lseek(fd, pos, SEEK_SET);                  // 해당 위치로 이동

            n = read(fd, &current, sizeof(struct record));    // 레코드 읽기 읽은 데이터를 current 에 저장
            if (n == sizeof(struct record)) {
                display_record(&current);              // 정상 읽기면 출력
            } else {
                printf("empty or invalid record\n");   // 데이터가 없으면 안내
            }

            reclock(fd, record_no, sizeof(struct record), F_UNLCK);
            // lock 해제
            break;

        case 'd':   // deposit
            reclock(fd, record_no, sizeof(struct record), F_WRLCK);
            // 해당 레코드에 write lock 획득

            pos = record_no * sizeof(struct record);   // 위치 계산
            lseek(fd, pos, SEEK_SET);                  // 해당 위치로 이동

            n = read(fd, &current, sizeof(struct record));    // 기존 내용 읽기
            if (n != sizeof(struct record)) {                 // 없으면 처리 불가
                printf("empty or invalid record\n");
                reclock(fd, record_no, sizeof(struct record), F_UNLCK);
                break;
            }

            display_record(&current);                  // 현재 내용 출력

            printf("enter amount\n");
            scanf("%d", &amount);                      // 입금 금액 입력
            fgets(buffer, sizeof(buffer), stdin);      // 개행 제거

            current.balance += amount;                 // 잔액 증가

            lseek(fd, pos, SEEK_SET);                  // 다시 같은 위치로 이동
            n = write(fd, &current, sizeof(struct record));   // 수정 내용 저장
            if (n != sizeof(struct record)) {
                perror("write");
            }

            reclock(fd, record_no, sizeof(struct record), F_UNLCK); // 락 점유 해제
            // lock 해제
            break;

        case 'q':   // quit
            quit = TRUE;
            break;

        default:
            printf("illegal input\n");                 // 잘못된 입력
            continue;
        }

        if (quit == TRUE)                              // q 입력 시 종료
            break;
    }

    close(fd);                                         // 파일 닫기
    fflush(NULL);                                      // 모든 출력 버퍼 비우기
    return 0;                                          // 정상 종료
}
//특정 파일에 락을 걸거나, 해제하는 메서드
int reclock(int fd, int recno, int len, int type)
{
    struct flock fl;                                   // 파일 락 구조체

    switch (type) {
    case F_RDLCK:                                      // read lock
    case F_WRLCK:                                      // write lock
    case F_UNLCK:                                      // unlock
        fl.l_type = type;                              // 락 종류
        fl.l_whence = SEEK_SET;                        // 파일 시작 기준
        fl.l_start = recno * len;                      // 시작 위치
        fl.l_len = len;                                // 잠글 길이(레코드 1개)

        if (fcntl(fd, F_SETLKW, &fl) == -1) {          // blocking lock 설정
            perror("fcntl");
            return -1;
        }
        return 1;

    default:
        return -1;
    }
}

void display_record(struct record *curr)
{
    printf("\n");
    printf("id: %d\n", curr->id);                      // id 출력
    printf("name: %s\n", curr->name);                  // 이름 출력
    printf("balance: %d\n", curr->balance);            // 잔액 출력
    printf("\n");
}