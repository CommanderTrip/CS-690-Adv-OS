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
    FILE *outputFile;
    outputFile = fopen("OutputFile", "a");
    fprintf(outputFile, "Thread%ld: %s begins\n", p->threadId, &p->processName);
    fclose(outputFile);

    // Loop wait time
    int numOfLoops = p->waitTime / 5;
    for (int i = 0; i < numOfLoops; i++) {
        outputFile = fopen("OutputFile", "a");
        fprintf(outputFile, "Thread%ld: Loop %d of %d loops total\n", p->threadId, i, numOfLoops);
        fclose(outputFile);
        usleep(5000);
    }

    // Close
    outputFile = fopen("OutputFile", "a");
    fprintf(outputFile, "Thread%ld: %s ends\n", p->threadId, &p->processName);
    fclose(outputFile);
    pthread_exit(NULL);
}

int main() {
    // Get input file
    char fileLocation[128];
    strcpy(fileLocation, "./InputFile");
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
