#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// test_function의 선언
void test_function(const char *input);

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // 입력 데이터를 문자열로 변환
    char *input = (char *)malloc(Size + 1);
    if (input == NULL) {
        return 0;
    }

    memcpy(input, Data, Size);
    input[Size] = '\0';

    // test_function 호출
    test_function(input);

    free(input);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        fprintf(stderr, "Cannot open %s\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *data = (uint8_t *)malloc(size);
    if (!data) {
        fclose(file);
        return 1;
    }

    fread(data, 1, size, file);
    fclose(file);

    LLVMFuzzerTestOneInput(data, size);

    free(data);
    return 0;
}
