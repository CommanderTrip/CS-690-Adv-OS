#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "stdlib.h"
#include <unistd.h>

struct Process {
    long threadId;
    char processName;
    long waitTime;
};

void *Scheduled_Process(void *process) {
    // Greet
    struct Process *p = (struct Process *)process;
    printf("Thread%ld: %s begins\n", p->threadId, &p->processName);

    // Loop wait time
    int numOfLoops = p->waitTime / 5;
    for (int i = 0; i < numOfLoops; i++) {
        printf("Thread%ld: Loop %d of %d loops total\n", p->threadId, i, numOfLoops);
        usleep(5000);
    }

    // Close
    printf("Thread%ld: %s ends\n", p->threadId, &p->processName);
    pthread_exit(NULL);
}

int main() {
    // Get input file
    printf("Please enter the input file location or \"1\" to assume \"./InputFile\"\n");
    char fileLocation[128];
    scanf("%s", fileLocation);
    if (strcmp(fileLocation, "1") == 0) strcpy(fileLocation, "./InputFile");
    FILE *inputFile = fopen(fileLocation,"r");
    if (inputFile == NULL) {
        printf("Unable to open file.\n");
        return -1;
    }

    // Get file contents
    char lineContent[32];
    struct Process processes[32];
    int index = 0;
    while (fgets(lineContent, sizeof(lineContent), inputFile)) {
        struct Process newProcess;
        char *token = strtok(lineContent, " "); // First Token of the line
        strcpy(&newProcess.processName, token);
        token = strtok(NULL, " "); // Second Token of the line
        char *endptr;
        newProcess.waitTime = strtol(token, &endptr, 10);
        printf("%ld\n", newProcess.waitTime);
        processes[index] = newProcess;
        index++;
    }

    // Create the threads and wait the time
    int numOfThreads = index;
    int referenceCounter;
    pthread_t threads[numOfThreads];

    for (long i = 0; i < numOfThreads; i++) {
        processes[i].threadId = i + 1; // Starting at 1
        referenceCounter = pthread_create(&threads[i], NULL, Scheduled_Process, (void *)&processes[i]);
        if (referenceCounter) {
            printf("Error\n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}
