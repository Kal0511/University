/*
* File Name: square.h
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#ifndef EXERCISE_B_SQUARE_H
#define EXERCISE_B_SQUARE_H

#include "point.h"
#include "shape.h"

class Square : public Shape {
public:
    Square(const double &x, const double &y, const double &sideLength, const char *name);

    const double area()const;

    const double  perimeter()const;

    void display() ;

//    double distance(Square &other);
//
//    static double distance(Square &the_shape, Square &other);

protected:
    double side_a;
};


#endif //EXERCISE_B_SQUARE_H
