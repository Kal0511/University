#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<cstdlib>
#include<vector>
#include<semaphore.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

#define NUM_CUSTOMERS 5
#define NUM_ROUNDS 3

struct Timer {
    double timeWaiting;
    pthread_mutex_t timeLock;

    void init() {
        timeWaiting = 0;
        timeLock = PTHREAD_MUTEX_INITIALIZER;
    }

    void addTime(double time) {
        pthread_mutex_lock(&timeLock);
        timeWaiting += time;
        pthread_mutex_unlock(&timeLock);
    }
    double getTime(){
        pthread_mutex_lock(&timeLock);
        return timeWaiting;
        pthread_mutex_unlock(&timeLock);
    }
};

struct WaiterMonitor {
    int peopleDoneEating;
    pthread_mutex_t countLock;

    pthread_mutex_t allowedToEat[NUM_CUSTOMERS];

    pthread_cond_t signalNextRound;

    sem_t leftChopsticks;
    sem_t rightChopsticks;

    void startShop(int numOfChopsticks) {
        peopleDoneEating = 0;
        countLock = PTHREAD_MUTEX_INITIALIZER;
        for (int i = 0; i < NUM_CUSTOMERS; i++) {
            allowedToEat[i] = PTHREAD_MUTEX_INITIALIZER;
        }
        signalNextRound = PTHREAD_COND_INITIALIZER;
        sem_init(&leftChopsticks, 0, numOfChopsticks / 2);
        sem_init(&rightChopsticks, 0, numOfChopsticks / 2 + numOfChopsticks % 2);
    }

    void stopShop() {
        sem_destroy(&leftChopsticks);
        sem_destroy(&rightChopsticks);
    }

    void getChopsticks() {
        sem_wait(&leftChopsticks);
        sem_wait(&rightChopsticks);
    }

    void putChopsticks() {
        sem_post(&leftChopsticks);
        sem_post(&rightChopsticks);
    }

    void eat() {
        sleep(5);
        pthread_mutex_lock(&countLock);
        peopleDoneEating++;
        pthread_cond_signal(&signalNextRound);
        pthread_mutex_unlock(&countLock);
    }

    void think() {
        sleep(2);
    }

    void awaitNextRound(int i) {
        pthread_mutex_lock(&allowedToEat[i]);
    }

    void startNextRound() {
        pthread_mutex_lock(&countLock);
        while (peopleDoneEating != NUM_CUSTOMERS) {
            pthread_cond_wait(&signalNextRound, &countLock);
        }
        peopleDoneEating = 0;
        for (int j = 0; j < NUM_CUSTOMERS; j++) {
            pthread_mutex_unlock(&allowedToEat[j]);
        }
        pthread_mutex_unlock(&countLock);
    }

};

struct WaiterMonitor waiter;
struct Timer timer;

void *customer(void *arg) {
    int index = *(int *) arg;
    for (int n = 0; n < NUM_ROUNDS; n++) {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        printf("Philosopher %d is Hungry\n", index);
        waiter.awaitNextRound(index);
        waiter.getChopsticks();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, std::milli> time_span = t2 - t1;
        timer.addTime(time_span.count());
        printf("Philosopher %d is Eating\n", index);
        waiter.eat();
        waiter.putChopsticks();
        printf("Philosopher %d is Thinking\n", index);
        waiter.think();
    }
    delete (int *) arg;
    pthread_exit(nullptr);
}

int main(int commandArgsLen, char **commandArgs) {
    int numOfChopsticks = 5;
    if (commandArgsLen > 1) {
        numOfChopsticks = (int) strtol(commandArgs[1], nullptr, 10);
        if (numOfChopsticks <= 1) {
            cout << "Invalid Input - Using Default" << endl;
            numOfChopsticks = 5;
        }
    }
    cout << "Num of Chopsticks: " << numOfChopsticks << endl;
    waiter.startShop(numOfChopsticks);
    timer.init();
    vector<pthread_t> threads;
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_t tid;
        int *arg = new int;
        *arg = i;
        pthread_create(&tid, nullptr, customer, arg);
        threads.push_back(tid);
    }
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("Round: %d\n", i);
        waiter.startNextRound();
    }
    waiter.stopShop();
    cout << timer.getTime() / (NUM_CUSTOMERS * NUM_ROUNDS) << " Milliseconds" << endl;
    return 0;
}