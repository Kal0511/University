/*
* File Name: curveCut.h
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#ifndef EXERCISE_A_CURVECUT_H
#define EXERCISE_A_CURVECUT_H

#include "circle.h"
#include "rectangle.h"

class CurveCut : public Circle, public Rectangle {
public:
    CurveCut(const double &x, const double &y, const double &sideA, const double &sideB, const double &radius, const char* name);
    CurveCut(const CurveCut &rhs);
    CurveCut& operator=(const CurveCut &rhs);

    const double area()const override;

    const double perimeter()const override;

    void display() override;


};


#endif //EXERCISE_A_CURVECUT_H
