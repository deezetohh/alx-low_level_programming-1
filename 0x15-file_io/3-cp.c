#include <stdio.h>
#include <stdlib.h>

void fileclose(FILE **file) {
    if (*file) {
        fclose(*file);
        *file = NULL;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: cp file_from file_to\n");
        return 1;
    }
    char *from_path = argv[1];
    char *to_path = argv[2];
    FILE *from_file = fopen(from_path, "rb");
    if (!from_file) {
        printf("Error: Can't read from file %s\n", from_path);
        return 1;
    }
    FILE *to_file = fopen(to_path, "wb");
    if (!to_file) {
        printf("Error: Can't write to file %s\n", to_path);
        fileclose(&from_file);
        return 1;
    }
    char buf[1024];
    size_t num_read;
    while ((num_read = fread(buf, 1, sizeof(buf), from_file)) > 0) {
        if (fwrite(buf, 1, num_read, to_file) != num_read) {
            printf("Error: Can't write to file %s\n", to_path);
            fileclose(&from_file);
            fileclose(&to_file);
            return 1;
        }
    }
    if (ferror(from_file)) {
        printf("Error: Can't read from file %s\n", from_path);
        fileclose(&from_file);
        fileclose(&to_file);
        return 1;
    }
    fileclose(&from_file);
    fileclose(&to_file);
    return 0;
}

