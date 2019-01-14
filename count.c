#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

long getSize(FILE* file);
long readAndCount(FILE* inputFile, char* searchString);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Error: invalid input\n");
        exit(1);
    }
    char* inputFileName = argv[1];
    char* searchString = argv[2];
    char* outputFileName = argv[3];

    FILE* inputFile;
    FILE* outputFile;

    /* open input File, get size and count, and close with error checking */
    if ((inputFile = fopen(inputFileName, "rb")) == NULL) {
        printf("Error: cannot open input file %s\n", inputFileName);
        exit(1);
    }
    long countNumber = readAndCount(inputFile, searchString);
    if (countNumber < 0) {
        perror("Error: ");
    }
    long size = getSize(inputFile);
    if (size < 0) {
        perror("Error: ");
    }
    if(fclose(inputFile) < 0) {
        perror("Error");
    }

    /* generating output information and output to file and screen */
    char records[100];
    sprintf(records,"Size of file is %ld\nNumber of matches is %ld\n", size, countNumber);
    printf("%s",records);
    if ((outputFile = fopen(outputFileName, "w+")) == NULL) {
        printf("Error: cannot open output file %s\n", outputFileName);
        exit(1);
    }
    if (fputs(records, outputFile) < 0){
        perror("Error: ");
    }
    if(fclose(outputFile) < 0) {
        perror("Error");
    }
    return 0;
}

/*
 * Function: getSize
 * ----------------------------
 *   Returns the size(Byte) of targeted file
 *
 *   file: targeted file
 *
 *   returns: tReturns the size(Byte) of file
 */
long getSize(FILE* file) {
    long size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}


/*
 * Function: greadAndCount
 * ----------------------------
 *   Read inputFile and count string which is searched.
 *
 *   inputFile: targeted file to be read
 *
 *   searchString: counted string
 *
 *   returns: the frequency of searchString in inputFile
 */
long readAndCount(FILE* inputFile, char* searchString) {
    size_t len = strlen(searchString);
    size_t chunkSize = 100;
    long count = 0;
    long offset = 0;
    unsigned char buffer[chunkSize];

    while (fread(buffer, 1, chunkSize, inputFile) > 0) {
        for (int i = 0; i < chunkSize - len; i++) {
            offset++;
            int j;
            for (j = 0; j < len; j++) {
                if (buffer[i + j] != (searchString[j] & 0xff)) {
                    break;
                }
            }
            if (j == len) {
                count++;
            }
        }
        /* clean the buffer after searching in one chunk, or it would cause extra results in last iteration */
        for (int i = 0; i < chunkSize; i++) {
            buffer[i] = 0;
        }

        /* reset the pointer for input file to allow matching from buffer[chunkSize - len,chunkSize] in next iteration */
        fseek(inputFile, offset, SEEK_SET);
    }
    return count;
}

