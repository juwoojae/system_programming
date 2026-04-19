#include <sys/stat.h>    // stat 구조체, chmod(), lstat()
#include <stdio.h>       // printf, perror

int main(int argc, char *argv[])
{
    struct stat statbuf;                         // 파일 정보 저장 구조체

    if(argc != 2) {                              // 인자 개수 체크
        perror("argument error");                // 에러 출력
        return 1;
    }

    if (lstat(argv[1], &statbuf) < 0) {          // 파일 정보 가져오기 (심볼릭 링크 그대로 확인)
        perror("lstat");
        return 2;
    }

    if (S_ISREG(statbuf.st_mode)) {              // 일반 파일인지 확인
        if (chmod(argv[1], (statbuf.st_mode & ~S_IXGRP)) < 0) {   // 그룹 실행 권한 제거
            perror("chmod");
            return 3;
        }
    }
    else
        printf("%s is not regular file\n", argv[1]);   // 일반 파일 아니면 출력
}
