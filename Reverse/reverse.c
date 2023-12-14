/************************************************************
 * This program is a simple reverse function which reverses given text file
 * or standard input stream to output file or to terminal.
 *
 * Program can be run in three ways:
 *  1) File input and no output: user can give first argument as text file 
 *     where program reads the lines and outputs lines in reverse order in terminal.
 *  2) File input and file output: user can give output file name where the
 *     lines from first text file will be reversed and saved to output file.
 *  3) No arguments: User can run program without additional arguments.
 *     program then reads from standard input stream and reverses given lines 
 *     to terminal after 'quit' is written to terminal.
 *
 * In addition to the course material and LUT programming guide
 * for the C-language courses, we found help for getline function and how it does 
 * memory allocation from: https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/
 * and help for linked list and going through it: https://www.learn-c.org/en/Linked_lists
 *
 * Authors:
 * - Aleksi Haapalainen
 * - Atte Hiltunen
 ************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// argc = how many files (should be 3)
// argv[] list of those arguments given

struct Node // linked list node
{
    char *Line;
    struct Node *next;
    struct Node *before;
};

void freeLinkedList(struct Node *end) // Frees memory of the whole linked list which was malloc'd
{
    while (end != NULL)
    {
        struct Node *temp = end;
        end = end->before;
        free(temp->Line);

        free(temp);
    }
}
struct Node *AddNode(struct Node **head, const char *Line)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        perror("malloc failed");
        exit(1);
    }

    // Allocate memory for the line and copy the content
    newNode->Line = strdup(Line);
    if (newNode->Line == NULL)
    {
        perror("malloc failed");
        free(newNode);
        exit(1);
    }

    // Set the next pointer to NULL
    newNode->next = NULL;

    // If the linked list is empty, set the new node as the head
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        // if linked list is not empty add the new node to the end of the list
        struct Node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        newNode->before = current;
        current->next = newNode;
    }
    // return the lates node
    return newNode;
}

struct Node *readToLinkedlist(FILE *inputFile) // function to read given input to linked list
{
    struct Node *head = NULL;
    struct Node *end = NULL;
    char *Line = NULL;
    size_t lineSize = 0;
    int isStdin = (inputFile == stdin);                  // check if we getting standard input or are we in a file
    while ((getline(&Line, &lineSize, inputFile) != -1)) // get line to compare
    {

        if (isStdin && strcmp(Line, "quit\n") == 0) // if we are in standardinput and qord is quit we exit the loop
        {

            break;
        }

        size_t len = strlen(Line) - 1;
        if (Line[len] == '\n')
        {
            Line[len] = '\0'; // replace newline with \0
        }
        end = AddNode(&head, Line);

        free(Line);
        Line = NULL; // Reset Line to NULL after freeing to avoid dangling pointer
        lineSize = 0;
    }
    if (Line != NULL) // Check if Line is not NULL before freeing
    {
        free(Line);
    }
    return end;
}

void WritetoFile(struct Node **end, char *filename) // start from end and write each linked list node to file
{
    struct Node *current = *end;
    FILE *outputfile = fopen(filename, "w");
    if (outputfile == NULL)
    {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(1);
    }
    while (current != NULL)
    {
        if (current->before == NULL) // if node is last(to output file) do not add \n to the end
        {
            fprintf(outputfile, "%s", current->Line);
        }
        else
        {
            fprintf(outputfile, "%s\n", current->Line);
        }
        current = current->before;
    }
    fclose(outputfile);
}

void printLinkedList(struct Node **end) // print the linked list in reverse order to terminal
{
    struct Node *current = *end;
    while (current != NULL)
    {
        printf("%s\n", current->Line);
        current = current->before;
    }
}
int main(int argc, char *argv[])
{
    char *Line = NULL;
    // struct Node *head = NULL;
    struct Node *end = NULL;
    size_t lineSize = 0;

    FILE *inputFile;
    if (argc > 3)
    {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]); // check that right amount of files are given
        exit(1);
    }
    if (argv[1] != NULL) // check if input file is given
    {
        inputFile = fopen(argv[1], "r");

        if (inputFile == NULL) // check that input file is correct
        {
            fprintf(stderr, "cannot open file '%s'\n", argv[1]);

            exit(1);
        }
        end = readToLinkedlist(inputFile);
        fclose(inputFile);
    }
    else
    {
        end = readToLinkedlist(stdin); // if no input file is given we read input from standard input stream
        printLinkedList(&end);
    }

    if (argv[2] != NULL && argv[1] != NULL) // check that output file is given where to write
    {

        if (strcmp(argv[1], argv[2]) == 0) // check that input and outputfile differ
        {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        WritetoFile(&end, argv[2]);
    }
    else
    {
        printLinkedList(&end);
    }

    freeLinkedList(end);
    // Free memory allocated by getline
    free(Line);
    // end the program with 0
    return 0;
}