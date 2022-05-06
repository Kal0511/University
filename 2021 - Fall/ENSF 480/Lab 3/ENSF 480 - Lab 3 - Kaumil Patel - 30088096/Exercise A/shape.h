/*
* File Name: shape.h
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#ifndef EXERCISE_B_SHAPE_H
#define EXERCISE_B_SHAPE_H

#include "point.h"

 class  Shape virtual{
public:
    Shape(const double &x, const double &y, const char *name);

    ~Shape();

    Shape &operator=(const Shape &other);

    const Point &getOrigin();

    char *getName();

    void setName(const char *name);

    virtual void display() ;

    const double distance(Shape &other);

    static const double distance(Shape &the_shape, Shape &other);

    void move(const double& dx, const double& dy);

    virtual const double area()const = 0;
    virtual const double perimeter()const = 0;

protected:
    Point origin;
    char *shapeName;
};


#endif //EXERCISE_B_SHAPE_H
