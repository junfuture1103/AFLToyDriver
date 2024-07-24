#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

void process_input(const char *input) {
    if (strncmp(input, "FUZZ", 4) == 0) {
        printf("Detected FUZZ prefix\n");

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

    if (strstr(input, "CRASH") != NULL) {
        printf("CRASH found in input, triggering error...\n");
        abort(); // This will cause a crash for testing
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char buffer[BUF_SIZE];
    if (fgets(buffer, BUF_SIZE, file) != NULL) {
        process_input(buffer);
    } else {
        perror("fgets");
    }

    fclose(file);
    return 0;
}
