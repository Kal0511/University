/*
* File Name: square.h
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#ifndef EXERCISE_A_CIRCLE_H
#define EXERCISE_A_CIRCLE_H


#include "shape.h"

class Circle :virtual public Shape {
public:
    Circle(const double &x, const double &y, const double &sideLength, const char *name);

    const double area()const override;

    const double  perimeter()const override;

    void display() override;

    void setradius(const double &radius);

    const double &getradius() const;


protected:
    double radius;
};


#endif //EXERCISE_A_CIRCLE_H
