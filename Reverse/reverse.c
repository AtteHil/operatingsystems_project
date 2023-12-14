#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// argc = how many files (should be 3)
// argv[] list of those arguments given
// help for getline function and how it does memory allocation https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/
// help for linked list and going through it: https://www.learn-c.org/en/Linked_lists
struct Node // linked list node
{
    char *Line;
    struct Node *next;
    struct Node *before;
};

void freeLinkedList(struct Node *head) // Frees memory of the whole linked list which was malloc'd
{
    while (head != NULL)
    {
        struct Node *temp = head;
        head = head->next;
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
void printLinkedList(struct Node **head)
{
    struct Node *current = *head;
    while (current != NULL)
    {
        printf("%s", current->Line);
        current = current->next;
    }
}
int main(int argc, char *argv[])
{
    char *Line = NULL;
    struct Node *head = NULL;
    struct Node *end = NULL;
    size_t lineSize = 0;

    FILE *inputFile;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]); // check that right amount of files are given
        exit(1);
    }
    if (strcmp(argv[1], argv[2]) == 0) // check that input and outputfile differ
    {
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }
    inputFile = fopen(argv[1], "r");

    if (inputFile == NULL) // check that input file is correct
    {
        fprintf(stderr, "cannot open file '%s'\n", argv[0]);
        fclose(inputFile);
        exit(1);
    }

    while (getline(&Line, &lineSize, inputFile) != -1) // getline memory allocates own memory for each line
    {
        size_t len = strlen(Line) - 1;
        if (Line[len] == '\n')
        {
            Line[len] = '\0'; // replace newline with \0
        }
        end = AddNode(&head, Line);
        free(Line);
        Line = 0;
        lineSize = 0;
    }
    fclose(inputFile); // close the input file
    WritetoFile(&end, argv[2]);
    freeLinkedList(head);
    // Free memory allocated by getline
    free(Line);
    // end the program with 0
    return 0;
}