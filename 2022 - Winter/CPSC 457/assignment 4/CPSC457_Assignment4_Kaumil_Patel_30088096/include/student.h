#ifndef ASSIGNMENT_4_STUDENT_H
#define ASSIGNMENT_4_STUDENT_H

#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

auto getCurrentTime() {
    return high_resolution_clock::now();
}

// data structure used to represent a student
struct student {
    int studentID;
    int relativeArrivalTime;
    int eatingDuration;

    time_point<high_resolution_clock> arrivalTime;
    time_point<high_resolution_clock> seatingTime;
    time_point<high_resolution_clock> departureTime;

    int turnaroundTime;
    int waitingTime;

    int ateForTime;

    student(const int &id, const int &arrival, const int &eating) : studentID(id), relativeArrivalTime(arrival), eatingDuration(eating),
                                                                    turnaroundTime(0), waitingTime(0), ateForTime(0) {}

    void print() const {
        cout << "ID: " << studentID << endl;
//        cout << "Relative Arrival Time: " << relativeArrivalTime << endl;
//        cout << "Eating Time: " << eatingDuration << endl;
//        cout << "Arrival Time: " << round(duration<double, milli>(arrivalTime - start).count() / 1000) << endl;
//        cout << "Seating Time: " << round(duration<double, milli>(seatingTime - start).count() / 1000) << endl;
//        cout << "Departure Time: " << round(duration<double, milli>(departureTime - start).count() / 1000) << endl;
        cout << "Turnaround Time: " << turnaroundTime << endl;
        cout << "Waiting Time " << waitingTime << endl << endl;
    }

    void arrived() {
        arrivalTime = getCurrentTime();
    }

    void seated() {
        seatingTime = getCurrentTime();
        waitingTime += (int) round(duration<double, milli>(seatingTime - arrivalTime).count() / 1000);
    }

    void left() {
        departureTime = getCurrentTime();
        turnaroundTime += (int) round(duration<double, milli>(departureTime - arrivalTime).count() / 1000);
    }

    void ateFor(const int &time) {
        ateForTime += time;
    }

    bool doneEating() const {
        return ateForTime == eatingDuration;
    }
} typedef student;

#endif
