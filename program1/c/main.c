#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "stdlib.h"

void *Scheduled_Process(void *threadid) {
    printf("Hello Thread\n");
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
    char fileContents[64][64];
    int index = 0;
    while (fgets(fileContents[index], sizeof(fileContents), inputFile)) {
        printf("%s", fileContents[index]);
        index++;
    }

    // Create the threads and wait the time
    int numOfThreads = index + 1;
    int referenceCounter;
    pthread_t threads[numOfThreads];

    for (long i = 0; i < numOfThreads; i++) {
        printf("Creating Thread %ld...\n", i);
        referenceCounter = pthread_create(&threads[i], NULL, Scheduled_Process, (void *)i);
        if (referenceCounter) {
            printf("Error\n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}
