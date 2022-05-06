/*
* File Name: square.h
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#ifndef EXERCISE_B_SQUARE_H
#define EXERCISE_B_SQUARE_H

#include "point.h"
#include "shape.h"

class Square :  virtual public Shape {
public:
    Square(const double &x, const double &y, const double &sideLength, const char *name);

    const double area()const override;

    const double  perimeter()const override;

    void display() override ;

//    double distance(Square &other);
//
//    static double distance(Square &the_shape, Square &other);

protected:
    double side_a;
};


#endif //EXERCISE_B_SQUARE_H
