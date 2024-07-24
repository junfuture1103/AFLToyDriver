#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

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

int main(int argc, char *argv[]) {
    printf("argc : %d argv[1] : %s\n", argc, argv[1]);

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
