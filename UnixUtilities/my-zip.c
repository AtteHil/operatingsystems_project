#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void my_zip(FILE *file) {
    int currentChar;
    int previousChar;
    int count = 1;

    // Read the first character in the file
    currentChar = fgetc(file);
    if (currentChar == EOF) {
        return;
    }

    // Loop through the rest of the file and count concurrent characters
    while (currentChar != EOF) {
        previousChar = currentChar;
        currentChar = fgetc(file);

        if (currentChar == previousChar) {
            count++;

        } else {
            // Write the count and character to stdout in binary format (integer followed by character)
            fwrite(&count, sizeof(int), 1, stdout);
            putchar(previousChar);

            count = 1;
        }
    }

    // Handle the last character outside of the loop
    fwrite(&count, sizeof(int), 1, stdout);
    putchar(previousChar);
    putchar('\n');
}


int main(int argc, char *argv[]) {
    // Check if the user provided less than 2 arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 [file2 ...]\n", argv[0]);
        return 1;
    }

    // If the first argument is "my-zip", open and compress files using my_zip function
    if (strcmp(argv[0], "./my-zip") == 0) {
        for (int i = 1; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("Error opening file");
                return 1;
            }
            my_zip(file);
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
