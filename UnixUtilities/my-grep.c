/************************************************************
 * This program is a simple grep function which finds if wanted word
 * is in given text. Program prints out found lines which include wanted string.
 *
 * Program can be run two ways
 *  1) File input: user can give first arguments 1. what string to look for
 *     text files (one or more) where the given word is looked for.
 *     Lines whhich include the word are printed to terminal.
 *  2) Standard input: user can leave arguments empty after wanted word.
 *     Program will let user write lines in terminal and look from those lines if given word is in it.
 *     program will print line if word is in it.
 *
 * In addition to the course material and LUT programming guide
 * for the C-language courses, we got help for comparing strings 
 * from: https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
 *
 * Authors:
 * - Aleksi Haapalainen
 * - Atte Hiltunen
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ReadAndCompare(FILE *inputFile, char *searched)
{
    char *Line = NULL;
    size_t lineSize = 0;
    int isStdin = (inputFile == stdin);                  // check if we getting standard input or are we in a file
    while ((getline(&Line, &lineSize, inputFile) != -1)) // get line to compare
    {

        if (isStdin && strcmp(Line, "quit\n") == 0) // if we are in standardinput and qord is quit we exit the program
        {

            break;
        }

        if (strstr(Line, searched) != NULL) // check if line includes searched word
        {

            printf("%s", Line);
        }

        free(Line);
        Line = NULL; // Reset Line to NULL after freeing to avoid dangling pointer
        lineSize = 0;
    }
    free(Line);
}

int main(int argc, char *argv[])
{
    FILE *inputFile = NULL;

    if (argc == 1)
    {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        return 1;
    }
    if (argc == 2) // if no file is given
    {
        ReadAndCompare(stdin, argv[1]);
        return 0;
    }
    for (int i = 2; i < argc; i++) // for loop to go through each file
    {
        inputFile = fopen(argv[i], "r"); // opening and checking the file
        if (inputFile == NULL)
        {
            fprintf(stderr, "my-cat: cannot open file %s\n", argv[i]);
            return 1;
        }
        ReadAndCompare(inputFile, argv[1]);
        fclose(inputFile);
    }
    return 0;
}