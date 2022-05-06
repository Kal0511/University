#include <iostream>
#include <fstream>
#include <mutex>
#include <sys/unistd.h>
#include <vector>
#include <cmath>

using namespace std;
using namespace std::chrono;

#define NUM_TABLES 4

#include "include/queue.h"
#include "include/student.h"

int maxEatingTime;
queue<student> diningQueue;
vector<student> students;

// Round Robin Table
void *startRoundRobinTable(void *arg) {
    while (true) {
        student *individual = diningQueue.pop();
        if(!individual){
            delete (int *) arg;
            pthread_exit(nullptr);
        }
        individual->seated();
        printf("Sit %d\n", individual->studentID);
        if (individual->eatingDuration - individual->ateForTime > maxEatingTime) {
            sleep(maxEatingTime);
            individual->ateFor(maxEatingTime);
            individual->left();
            individual->arrived();
            diningQueue.push(individual);
            printf("Preempt %d\n", individual->studentID);
        } else {
            sleep(individual->eatingDuration - individual->ateForTime);
            individual->ateFor(individual->eatingDuration - individual->ateForTime);
            individual->left();
            printf("Leave %d Turnaround %d Wait %d\n", individual->studentID, individual->turnaroundTime, individual->waitingTime);
        }
    }
}

// reads data file and add students based on the data
void *producer(void *arg) {
    char *filename = (char *) arg;
    string line;
    ifstream file;
    string dir = "data/";
    file.open(dir.append(filename));
    if (file.is_open()) {
        getline(file, line);
        maxEatingTime = stoi(line);
        int i = 0;
        while (getline(file, line)) {
            string::size_type pos;
            int a = stoi(line, &pos);
            int e = stoi(line.substr(pos));
            students.emplace_back(i, a, e);
            i++;
        }
        cout << endl;
    }else{
        printf("Error opening file\n");
        exit(1);
    }
    file.close();

    for (int i = 0; i < students.size(); i++) {
        sleep(students[i].relativeArrivalTime);
        students[i].arrived();
        printf("Arrive %d\n", students[i].studentID);
        diningQueue.push(&students[i]);
    }

    pthread_exit(nullptr);
}

int main(int argc, char *argv[]) {
    // read in file name
    char const *filename = "data.txt";
    if (argc < 2) {
        printf("No input: using default file - data.txt\n");
    } else {
        filename = argv[1];
    }

    // start restaurant tables
    vector<pthread_t> threads;
    for (int i = 0; i < NUM_TABLES; i++) {
        pthread_t tid;
        int *arg = new int;
        *arg = i;
        pthread_create(&tid, nullptr, startRoundRobinTable, arg);
        threads.push_back(tid);
    }

    // start student producer
    pthread_t pid;
    pthread_create(&pid, nullptr, producer, (void *) filename);
    pthread_join(pid, nullptr);

    // close the queue and empty it
    diningQueue.closeQueue();
    diningQueue.refresh();

    // await for the tables to stop
    for (int i = 0; i < NUM_TABLES; i++) {
        pthread_join(threads[i], nullptr);
    }

    cout << endl;

//    for (int i = 0; i < students.size(); i++) {
//        students[i].print();
//    }

    double totalTurnaroundTime = 0;
    double totalWaitingTime = 0;
    for (int i = 0; i < students.size(); i++) {
        totalTurnaroundTime += students[i].turnaroundTime;
        totalWaitingTime += students[i].waitingTime;
    }
    cout << "Average Turnaround Time: " << totalTurnaroundTime / students.size() << endl;
    cout << "Average Waiting Time " << totalWaitingTime / students.size() << endl;

    return 0;
}
