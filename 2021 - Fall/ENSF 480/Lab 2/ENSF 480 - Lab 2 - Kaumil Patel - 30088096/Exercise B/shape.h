/*
* File Name: shape.h
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#ifndef EXERCISE_B_SHAPE_H
#define EXERCISE_B_SHAPE_H

#include "point.h"

class Shape {
public:
    Shape(const double &x, const double &y, const char *name);

    ~Shape();

    Shape &operator=(const Shape &other);

    const Point &getOrigin();

    char *getName();

    virtual void display() ;

    const double distance(Shape &other);

    static const double distance(Shape &the_shape, Shape &other);

    void move(const double& dx, const double& dy);

protected:
    Point origin;
    char *shapeName;
};


#endif //EXERCISE_B_SHAPE_H
