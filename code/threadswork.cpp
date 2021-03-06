#include "threadswork.h"

//  Codes given piece of text to corresponding int sequence.
//  A -> 0; B -> 1; C -> 2; ... Z->25; a -> 26; b - > 27 ... z -> 51.
//  Returns false if encounters symbol that is not a letter.
void encryptText(const char *text, int *answer, int size) {
    for (int i = 0; i < size; ++i) {
        int letter_number = static_cast<int>(text[i]);
        //  If letter is low_case.
        if (letter_number >= 97) {
            letter_number -= 6;
        }
        letter_number -= 65;
        answer[i] = letter_number;
    }
}

void oneThreadTask(const char *text, int *coded, int length) {
    printf("One thread program started working\n");
    encryptText(text, coded, length);
    printf("One thread program finished working\n");
}

void* threadFunc(void *input) {
    ThreadsWork *threadsInput = (ThreadsWork *) input;
    printf("Thread %d started to work.\n", threadsInput->threadNumber);

    encryptText(threadsInput->text, threadsInput->coded, threadsInput->size);

    printf("Thread %d finished working.\n", threadsInput->threadNumber);
    return nullptr;
}

void multipleThreadTask(const char *text, int *coded, int length) {
    pthread_t thread[threadNumber];
    ThreadsWork args[threadNumber];
    int size_for_thread = length / threadNumber;
    int residue = length % threadNumber;

    clock_t start_time = clock();

    //  Creating threads.
    for (int i = 0; i < threadNumber; i++) {
        // Arguments for function in threads.
        args[i] = ThreadsWork{coded + size_for_thread, text + size_for_thread,
                              size_for_thread + (i == threadNumber) * residue, i};
        pthread_create(&thread[i], nullptr, threadFunc, (void *) (&args[i]));
    }
    clock_t end_time = clock();
    //  Waiting for all threads to finish.
    for (int i = 0; i < threadNumber; i++) {
        pthread_join(thread[i], nullptr);
    }
    clock_t end_work_time = clock();

    printf("Creation threads time - %f s.\n", double(end_time - start_time)/CLOCKS_PER_SEC);
    printf("Working threads time - %f s.\n", double(end_work_time - end_time)/CLOCKS_PER_SEC);
}
