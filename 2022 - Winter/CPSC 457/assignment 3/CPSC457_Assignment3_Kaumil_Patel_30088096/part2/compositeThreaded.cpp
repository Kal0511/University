#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define ARRAY_SIZE 1000000

bool isComposite(int n) {
    if (n <= 3) return false;
    if (n % 2 == 0 || n % 3 == 0) {
        return true;
    }
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return true;
        }
    }
    return false;
}

int nums[ARRAY_SIZE];

struct args_struct {
    int start;
    int end;
} typedef args_struct;

void *thread(void *arguments) {
    args_struct *args = (args_struct*)arguments;
//    printf("Start: %d | End: %d\n", args->start, args->end);
    for (int i = args->start; i < args->end; i++) {
        if (isComposite(nums[i])) {
//            printf("%d is a composite number\n", nums[i]);
        } else {
//            printf("%d is a prime number\n", nums[i]);
        }
    }
    pthread_exit(nullptr);
}

int main(int commandArgsLen, char **commandArgs) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        nums[i] = i;
    }
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    int numOfThreads = 10;
    if (commandArgsLen > 1) {
        numOfThreads = (int)strtol(commandArgs[1]+1, nullptr, 10);
        if (numOfThreads <= 0) {
            cout << "Invalid Input - Using Default" << endl;
            numOfThreads = 10;
        }
    }
    cout << "Number of Threads: " << numOfThreads << endl;
    pthread_t threads[numOfThreads];
    args_struct args[numOfThreads];
    for (int i = 0; i < numOfThreads; i++) {
        args[i].start = i * ARRAY_SIZE / numOfThreads;
        args[i].end = (i + 1) * ARRAY_SIZE / numOfThreads;
        pthread_create(&threads[i], nullptr, thread, &args[i]);
    }
    for (int i = 0; i < numOfThreads; i++) {
        pthread_join(threads[i], nullptr);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    cout << time_span.count() << " Milliseconds" << endl;
    return 0;
}


