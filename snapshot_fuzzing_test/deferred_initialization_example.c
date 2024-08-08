// gcc -o snapshot_fuzzing_example snapshot_fuzzing_example.c -no-pie
// There is no progress 8.8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <stdint.h>

#define SNAPSHOT_SIZE 0x10000 // 스냅샷 크기 정의

void *snapshot_memory = NULL; // 스냅샷 메모리 저장을 위한 포인터
void *target_memory = NULL; // 대상 메모리 블록 포인터

// 스냅샷 생성 함수
void snapshot_create() {
    if (snapshot_memory == NULL) {
        snapshot_memory = malloc(SNAPSHOT_SIZE);
        if (snapshot_memory == NULL) {
            perror("Failed to allocate snapshot memory");
            exit(1);
        }
    }
    memcpy(snapshot_memory, target_memory, SNAPSHOT_SIZE);
    printf("Snapshot created\n");
}

void initialize_process() {
    printf("Initializing process... (This will take at least 5 seconds)\n");
    sleep(5); // 5초 동안 대기
    // 추가적인 초기화 작업을 여기에 작성할 수 있습니다.
    printf("Initialization complete.\n");
}

// 스냅샷 복원 함수
void snapshot_restore() {
    if (snapshot_memory != NULL) {
        memcpy(target_memory, snapshot_memory, SNAPSHOT_SIZE);
        printf("Snapshot restored\n");
    } else {
        printf("No snapshot to restore\n");
    }
}

void process_input(const char *input) {
    printf("process input : %s\n", input);

    if (strncmp(input, "FUZZ", 3) == 0) {
        printf("Detected FUZZ prefix\n");

        if (strstr(input, "FUZZ_C") != NULL) {
            printf("CRASH found in input, triggering error...\n");
            abort(); // This will cause a crash for testing
        }

        if (strcmp(input, "FUZZ_AFL") == 0) {
            printf("Exact match for FUZZ_AFL\n");
        } else if (strcmp(input, "FUZZ_TEST") == 0) {
            printf("Exact match for FUZZ_TEST\n");
        } else {
            printf("FUZZ prefix but not an exact match\n");
        }
    } else {
        printf("No FUZZ prefix\n");
    }

}

int main(int argc, char **argv) {
    // 대상 메모리 블록 할당 및 초기화
    target_memory = mmap(NULL, SNAPSHOT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (target_memory == MAP_FAILED) {
        perror("Failed to allocate target memory");
        exit(1);
    }
    memset(target_memory, 0, SNAPSHOT_SIZE);

    // 스냅샷 생성
    printf("create snaphost!\n");
    printf("start fuzzing process\n");

    initialize_process();
    snapshot_create();

#ifdef __AFL_HAVE_MANUAL_CONTROL
  __AFL_INIT();
#endif

    while (__AFL_LOOP(1000)) {
        // 스냅샷 복원
        snapshot_restore();

        // 입력 파일에서 데이터 읽기
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Failed to open input file");
            exit(1);
        }

        char buf[100];
        size_t len = fread(buf, 1, sizeof(buf) - 1, file);
        fclose(file);

        if (len > 0) {
            buf[len] = '\0';
            // 입력 처리 코드
            process_input(buf);

            // 타겟 메모리 블록에 일부 데이터 변경 (예제)
            memcpy(target_memory, buf, len < SNAPSHOT_SIZE ? len : SNAPSHOT_SIZE);
        }

        // 루프가 종료되지 않도록 설정 (무한 루프)
        exit(1);
    }

    // 종료 코드
    munmap(target_memory, SNAPSHOT_SIZE);
    if (snapshot_memory != NULL) {
        free(snapshot_memory);
    }

    return 0;
}
