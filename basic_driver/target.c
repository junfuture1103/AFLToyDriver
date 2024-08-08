#include <stdio.h>
#include <string.h>

void test_function(const char *input) {
    if (strcmp(input, "fuzzing") == 0) {
        printf("Fuzzing is fun!\n");
    } else {
        printf("Try again.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        return 1;
    }

    test_function(argv[1]);
    return 0;
}
