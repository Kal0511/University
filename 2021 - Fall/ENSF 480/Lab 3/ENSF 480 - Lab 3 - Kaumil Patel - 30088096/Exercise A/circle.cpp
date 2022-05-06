/*
* File Name: square.cpp
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#include "iostream"
#include "iomanip"
#include <cmath>

using namespace std;

#include "circle.h"

Circle::Circle(const double &x, const double &y, const double &radius, const char *name) : Shape(x, y, name),
                                                                                               radius(radius) {}

void Circle::display() {
    cout << setprecision(2) << fixed;
    cout << "Circle Name: " << shapeName << endl;
    cout << "X-coordinate: " << origin.getx() << endl;
    cout << "Y-coordinate: " << origin.gety() << endl;
    cout << "Radius: " << radius << endl;
    cout << "Area: " << area() << endl;
    cout << "Perimeter: " << perimeter() << endl;
}

const double  Circle::perimeter() const{
    return M_PI * 2 * radius;
}

const double  Circle::area()const {
    return M_PI * radius * radius;
}

void Circle::setradius(const double &radius) {
    this->radius = radius;
}

const double &Circle::getradius() const {
    return radius;
}


