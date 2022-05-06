//
// Created by spdmo on 10/20/2021.
//

#ifndef EXERCISE_C_VEHICLE_H
#define EXERCISE_C_VEHICLE_H

#include "moveable.h"
#include "resizeable.h"

class Vehicle: public Moveable, public Resizeable{
protected:
    string name;
public:
    Vehicle(string name);
    virtual void move() = 0;
    void enlarge(int n) final;
    void shrink(int n) final;
    void forward() final;
    void backward() final;
};

#endif //EXERCISE_C_VEHICLE_H
