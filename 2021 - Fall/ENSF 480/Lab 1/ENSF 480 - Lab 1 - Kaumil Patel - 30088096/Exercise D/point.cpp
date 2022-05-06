/*
* File Name: point.cpp
* Assignment: Lab 1 Exercise D
* Completed by: Kaumil Patel
* Submission Date: Sept 23, 2020
*/

#include "point.h"

Point::Point(const double& a, const double& b) : x(a), y(b) {};

const double& Point::get_x() const {
    return x;
}

const double& Point::get_y() const {
    return y;
}

void Point::set_x(const double& a) {
    x = a;
}

void Point::set_y(const double& a) {
    y = a;
}