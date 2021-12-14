#ifndef ABC5_THREADSWORK_H
#define ABC5_THREADSWORK_H

#include <ctime>
#include <stdio.h>
//#include <pthread.h>

pthread_mutex_t mutex;
const int threadNumber = 4;

struct ThreadsWork {
    int *coded;
    const char *text;
    int size;
    int threadNumber;
};

//  Codes given piece of text to corresponding int sequence.
//  A -> 0; B -> 1; C -> 2; ... Z->25; a -> 26; b - > 27 ... z -> 51.
//  Returns false if encounters symbol that is not a letter.
void encryptText(const char *text, int *answer, int size);

//  The task is executed by one thread.
void oneThreadTask(const char *text, int *coded, int length);

//  Function that will be performed by threads.
//  To pass as argument in create threads function.
void threadFunc(void *input);

//  Executed by multiple threads when number of tasks evenly statically distributed.
void multipleThreadTask(const char *text, int *coded, int length);

#endif
