/************************************************************
 * This program is a simple cat function which prints given textfile to
 * terminal.
 *
 * Program accepts one or more text files and will print those out in
 * terminal.
 *
 *  the course material and LUT programming guide
 *  for the C-language course was used for source.
 *
 * Authors:
 * - Aleksi Haapalainen
 * - Atte Hiltunen
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1) // checkc that if not any files are given.
    {
        return 0;
    }
    FILE *inputFile;
    int c;
    for (int i = 1; i < argc; i++) // for loop to go through each file
    {
        inputFile = fopen(argv[i], "r"); // opening and checking the file
        if (inputFile == NULL)
        {
            fprintf(stderr, "my-cat: cannot open file %s\n", argv[i]);
            return 1;
        }
        while ((c = fgetc(inputFile)) != EOF) // while loop to print out each character in the file
        {
            printf("%c", c);
        }
        fclose(inputFile); // closing the file and adding newline so nect file doesn't start from same row.
        printf("\n");
    }

    return 0; // succesfull return
}