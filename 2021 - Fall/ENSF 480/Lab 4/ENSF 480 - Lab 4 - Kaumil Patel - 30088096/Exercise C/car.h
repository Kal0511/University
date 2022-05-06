//
// Created by spdmo on 10/20/2021.
//

#ifndef EXERCISE_C_CAR_H
#define EXERCISE_C_CAR_H

#include "vehicle.h"

class Car: public Vehicle{
private:
    int seats;
public:
    void turn();
    void move() final;
};

#endif //EXERCISE_C_CAR_H
