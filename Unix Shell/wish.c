#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_PATHS 64

// Function Declarations
void getError();
void parseUserInput(char *input, char *arguments[]);
void executeCommand(char *arguments[]);
void setCustomPaths(char *arguments[]);
void initializePaths();
void processInteractive(FILE *inputFile);
void processCommand(char *userArguments[], int *pathCheck);
void processCd(char *userArguments[]);
void processExternalCommand(char *userArguments[], int *pathCheck);
void processBatch(FILE *inputFile);


char* customPaths[MAX_PATHS];
char* originalPaths[MAX_PATHS];

int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 2) {
        fprintf(stderr, "Usage: %s [batch_file]\n", argv[0]);
        exit(1);
    }

    int interactive = (argc == 1);
    FILE *inputFile = stdin;

    if (!interactive) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            getError();
            exit(1);
        }
    }

    initializePaths();

    if (interactive) {
        processInteractive(inputFile);
    } else {
        processBatch(inputFile);
    }

    return 0;
}


void getError() {
    char msg[30] = "An error has occurred\n";
    write(STDERR_FILENO, msg, strlen(msg)); 
}


void parseUserInput(char *input, char *arguments[]) {
    char *token = strtok(input, " \t\n");
    int i;

    for (i = 0; token != NULL; i++) {
        arguments[i] = token;
        token = strtok(NULL, " \t\n");
    }
    arguments[i] = NULL;
}


void executeCommand(char *arguments[]) {
    pid_t processID = fork();

    if (processID == -1) {
        getError();
        return;
    }

    if (processID == 0) {
        execvp(arguments[0], arguments);

        getError();
        exit(1);
    }

    int status;
    waitpid(processID, &status, 0);
}


void setCustomPaths(char *arguments[]) {
    int i;
    for (i = 1; arguments[i] != NULL; i++) {
            customPaths[i - 1] = arguments[i];
        }
        customPaths[i - 1] = NULL;
    }


void initializePaths() {
    customPaths[0] = "/bin";

    for (int i = 0; customPaths[i] != NULL; i++) {
        originalPaths[i] = strdup(customPaths[i]);
    }
    originalPaths[MAX_PATHS - 1] = NULL;
}


void processInteractive(FILE *inputFile) {
    char userInput[MAX_INPUT];
    char *userArguments[MAX_ARGS];
    int pathCheck = 0;
    while (1) {
        printf("wish> ");

        if (fgets(userInput, sizeof(userInput), inputFile) == NULL) {
            if (inputFile != stdin) {
                fclose(inputFile);
            }
            exit(0);
        }

        parseUserInput(userInput, userArguments);
        if (userArguments[0] == NULL) {
            continue;
        }

        processCommand(userArguments, &pathCheck);
    }
}


void processCommand(char *userArguments[], int *pathCheck) {
    if (strcmp(userArguments[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(userArguments[0], "cd") == 0) {
        processCd(userArguments);
    } else if (strcmp(userArguments[0], "path") == 0) {
        setCustomPaths(userArguments);
    } else {
        processExternalCommand(userArguments, pathCheck);
    }
}


void processCd(char *userArguments[]) {
    if (userArguments[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else if (chdir(userArguments[1]) != 0) {
        getError();
    }
}


void processExternalCommand(char *userArguments[], int *pathCheck) {
    int i = 0;
    while (customPaths[i] != NULL) {
        char commandPath[MAX_PATHS];
        snprintf(commandPath, MAX_PATHS, "%s/%s", customPaths[i], userArguments[0]);
        if (access(commandPath, X_OK) == 0) {
            executeCommand(userArguments);
            *pathCheck = 1;
            break;
        }
        i++;
    }
    if (*pathCheck == 0) {
        fprintf(stderr, "Command not recognized.\n");
    }
    *pathCheck = 0;

    for (int j = 0; originalPaths[j] != NULL; j++) {
        customPaths[j] = strdup(originalPaths[j]);
    }
    customPaths[MAX_PATHS - 1] = NULL;
}


void processBatch(FILE *inputFile) {
    char userInput[MAX_INPUT];
    char *userArguments[MAX_ARGS];
    int pathCheck = 0;

    while (fgets(userInput, sizeof(userInput), inputFile) != NULL) {
        parseUserInput(userInput, userArguments);
        if (userArguments[0] == NULL) {
            continue;
        }

        processCommand(userArguments, &pathCheck);
    }

    fclose(inputFile);
    exit(0);
}

