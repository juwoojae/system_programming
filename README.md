# system-program

시스템 프로그래밍 개념을 C 예제로 정리한 학습용 저장소입니다.

## 파일 목록

- `CMakeLists.txt`: 현재 `process_start_exit/daemon.c`를 빌드 대상으로 잡는 기본 CMake 설정입니다.
- `thread_programming/`: pthread 스레드 생성, 종료, 취소, mutex 기반 동기화 예제가 들어 있습니다.
- `condition_variable/`: 조건변수와 producer-consumer 패턴 예제가 들어 있습니다.
- `concurrnent_processes/`: 파이프와 `fork`를 이용한 프로세스 간 통신 예제가 들어 있습니다.
- `process_start_exit/`: 프로세스 시작/종료 흐름과 함수 포인터 관련 예제가 들어 있습니다.
- `process_id/`: PID, PPID, 프로세스 그룹 등 프로세스 식별 정보 예제가 들어 있습니다.
- `record_lock/`: 레코드 단위 파일 접근과 `fcntl` 잠금 예제가 들어 있습니다.
- `standard_io_library/`: `fopen`, `fread`, `ferror`, `feof` 등 표준 입출력 라이브러리 예제가 들어 있습니다.
- `system_call/`: `open`, `creat`, `chmod`, `chown`, `dup2`, `link` 등 시스템 콜 예제가 들어 있습니다.

## 관련 공식 문서

- [pthread 공식 문서](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)
- [pthread_cond_wait](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3.html)
- [pthread_cancel](https://man7.org/linux/man-pages/man3/pthread_cancel.3.html)
- [fcntl record lock](https://man7.org/linux/man-pages/man2/fcntl.2.html)
- [fread](https://man7.org/linux/man-pages/man3/fread.3.html)
