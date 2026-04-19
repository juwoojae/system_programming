#include <sys/types.h>   // uid_t, gid_t 같은 타입 정의
#include <unistd.h>      // chown, fchown, lchown 함수
#include <stdio.h>       // printf, perror
#include <fcntl.h>       // open
#include <stdlib.h>   // atoi
#include <string.h>   // strcmp

int main(int argc, char *argv[])
{
    int owner_id, group_id, filedes;   // owner UID, group GID, 파일 디스크립터

    if(argc != 5) {                    // 인자 개수 체크
        perror("argument error");
        return 1;
    }

    owner_id = atoi(argv[3]);          // owner UID 변환
    group_id = atoi(argv[4]);          // group GID 변환

    // ---------------- chown ----------------
    if (strcmp(argv[1], "chown") == 0) {
        if (chown(argv[2], owner_id, group_id)) {   // 파일 경로 기준으로 소유자 변경
            perror("chown");
            return 2;
        }
        printf("chown %s to %s, %s\n", argv[2], argv[3], argv[4]);
    }

    // ---------------- fchown ----------------
    else if (strcmp(argv[1], "fchown") == 0) {
        filedes = open(argv[2], O_RDWR);            // 파일 열어서 fd 얻음

        if (fchown(filedes, owner_id, group_id)) { // fd 기반으로 소유자 변경
            perror("chown");
            return 3;
        }
        printf("fchown %s to %s, %s\n", argv[2], argv[3], argv[4]);
    }

    // ---------------- lchown ----------------
    else if (strcmp(argv[1], "lchown") == 0) {
        if (lchown(argv[2], owner_id, group_id)) { // 심볼릭 링크 자체의 소유자 변경
            perror("lchown");
            return 4;
        }
        printf("lchown %s to %s, %s\n", argv[2], argv[3], argv[4]);
    }
}
