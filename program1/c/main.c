#include "stdio.h"
#include "string.h"

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
    char fileContents[64];
    while (fgets(fileContents, sizeof(fileContents), inputFile)) {
        printf("%s", fileContents);
    }

    return 0;
}
