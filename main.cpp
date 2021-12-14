#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "ThreadsWork.h"


const int Max_Size = 214748360;

void errMessage1() {
    printf("%s", "incorrect command line!\n"
                 "  Waited:\n"
                 "     command -f infile outfile\n"
                 "  Or:\n"
                 "     command -n outfile\n");
}

void errMessage2() {
    printf("%s", "incorrect qualifier value!\n"
                 "  Waited:\n"
                 "     command -f infile outfile\n"
                 "  Or:\n"
                 "     command -n outfile\n");
}

//  Reads text from the given file.
//  Stops reading if either Max_Size characters are read or end of the file is reached.
int readText(char **text, FILE *file) {
    char buffer_char;
    char *buffer_string = new char[Max_Size];
    int i = 0;
    while (fscanf(file, "%c", &buffer_char) != EOF && i < Max_Size - 1) {
        buffer_string[i] = buffer_char;
        //  Checking if input data is valid.
        if (buffer_char < 65 || (buffer_char > 90 && buffer_char < 97) || buffer_char > 122) {
            return -1;
        }
        ++i;
    }
    (*text) = new char[i + 1];
    for (int j = 0; j < i; ++j) {
        (*text)[j] = buffer_string[j];
    }
    (*text)[i] = '\0';
    delete[] buffer_string;
    return i;
}

//  Generates sequence of n Latin letters.
void generateText(char **text, int n) {
    (*text) = new char[n + 1];
    for (int i = 0; i < n; ++i) {
        //  [65;90]u[97;122]
        (*text)[i] = static_cast<char>(rand() % 26 + 97 - (rand() % 2) * 32);
    }
    (*text)[n] = '\0';
}

//  Restores original sequence of letters from numbers.
void decryptText(const int *codes, char *answer, int size) {
    for (int i = 0; i < size; ++i) {
        answer[i] = static_cast<int>(codes[i] + 97 - 32 * (codes[i] < 26) - 26 * (codes[i] >= 26));
    }
    answer[size] = '\0';
}

void checkResult(const int *answer, const char *initial, int length) {
    if (length < 100) {
        printf("Encrypted text:\n");
        for (int i = 0; i < length; ++i) {
            printf("%d ", answer[i]);
        }
        printf("\n");
    }
    char *buffer = new char[length + 1];
    decryptText(answer, buffer, length);
    if (length < 100) {
        printf("Decrypted text:\n");
        printf("%s\n", buffer);
    }
    bool correct = true;
    for (int i = 0; i < length; ++i) {
        if (initial[i] != buffer[i]) {
            correct = false;
            printf("Mismatch found at the position %d.\n", i);
        }
    }
    if (correct) {
        printf("No mismatches found.\n");
    }
    delete[] buffer;
}

// out -f in.txt out.txt
// out -n symbolsNumber out.txt
int main(int argc, char **argv) {
    argc = 4;
    argv = new char *[argc];
    argv[1] = "-f";
    argv[2] = "in.txt";
    argv[3] = "out.txt";



    //  Check for correct command line.
    if (argc != 4) {
        errMessage1();
        return 1;
    }
    char *text;
    int length;
    if (!strcmp(argv[1], "-f")) {
        FILE *file = fopen(argv[2], "r");
        if (file) {
            length = readText(&text, file);
            if (length < 0) {
                printf("Incorrect input data in file. Only Latin letters are allowed.\n");
                fclose(file);
                return 5;
            }
        } else {
            printf("%s", "Failed to open input file.\n");
            fclose(file);
            return 3;
        }
        fclose(file);
    } else if (!strcmp(argv[1], "-n")) {
        length = atoi(argv[2]);
        if (length > Max_Size || length < 1) {
            printf("Incorrect number of symbols. Number has to be in interval (0;%d]\n", Max_Size);
            return 4;
        }
        generateText(&text, length);
    } else {
        errMessage2();
        return 2;
    }

    int *coded = new int[length];
    //  Work of one thread option.
    clock_t start_time = clock();  // Start time for one thread program.
    oneThreadTask(text, coded, length);
    clock_t end_time = clock();   // End time for one thread program.
    printf("Work time of one thread program - %d ms\n", end_time - start_time);
    printf("Checking started.\n");
    checkResult(coded, text, length);
    printf("***\n");

    //  Program with multiple number of threads.
    multipleThreadTask(text, coded, length);
    printf("Checking started.\n");
    checkResult(coded, text, length);


    if (FILE *file = fopen(argv[3], "w")) {
        for (int i = 0; i < length; ++i) {
            fprintf(file, "%d ", coded[i]);
        }
        fclose(file);
    }

    delete[] coded;
    delete[] text;
    return 0;
}
