#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void my_unzip(FILE *file) {
    int count;
    char currentChar;

    // Read the first integer in the file and 
    while (fread(&count, sizeof(int), 1, file) == 1) {
        // Read the character and repeat it count times
        currentChar = fgetc(file);
        for (int i = 0; i < count; i++) {
            putchar(currentChar);
        }
    }
    putchar('\n');
}


int main(int argc, char *argv[]) {
    // Check if the user provided less than 2 arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 [file2 ...]\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[0], "./my-unzip") == 0) {
        for (int i = 1; i < argc; i++) {
            FILE *file = fopen(argv[i], "rb");
            if (!file) {
                perror("Error opening file");
                return 1;
            }
            my_unzip(file);
            fclose(file);
        }  
    }
    // If the first argument is neither "my-zip" nor "my-unzip", print an error message 
    else {
        fprintf(stderr, "Unknown command\n");
        return 1;
    }

    return 0;
}