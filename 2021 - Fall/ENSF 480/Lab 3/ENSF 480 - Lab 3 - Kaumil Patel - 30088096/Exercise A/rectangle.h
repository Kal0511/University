/*
* File Name: rectangle.h
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021*/

#ifndef EXERCISE_B_RECTANGLE_H
#define EXERCISE_B_RECTANGLE_H

#include "square.h"

class Rectangle : public Square{
public:
    Rectangle(const double &x, const double &y, const double &sideA, const double &sideB, const char* name);
    void set_side_a(const double& i);
    void set_side_b(const double& i);
    void display() override;

    const double perimeter()const override;

    const double area()const override;

//    double distance(Rectangle &other);
//
//    double distance(Rectangle &the_shape, Rectangle &other);
protected:
    double side_b;
};


#endif //EXERCISE_B_RECTANGLE_H
