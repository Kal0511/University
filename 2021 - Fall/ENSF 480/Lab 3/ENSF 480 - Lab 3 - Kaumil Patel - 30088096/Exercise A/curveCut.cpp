/*
* File Name: curveCut.cpp
* Assignment: Lab 3 Exercise A
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#include "iostream"
#include "iomanip"
#include <cassert>

using namespace std;

#include "curveCut.h"

//CurveCut::CurveCut(const double &x, const double &y, const double &sideA, const double &sideB, const double &radius,
//                   const char *name) : Shape(x, y, name), Square(x, y, sideA, name),
//                                       Rectangle(x, y, sideA, sideB, name),
//                                       Circle(x, y, radius, name) {
//    if (radius > sideA || radius > sideB) {
//        cout << "Error: radius is longer than side lengths" << endl;
//    }
//    assert(radius <= sideA && radius <= sideB);
//}
CurveCut::CurveCut(const double &x, const double &y, const double &sideA, const double &sideB, const double &radius,
                   const char *name) : Shape(x, y, name), 
                                       Rectangle(x, y, sideA, sideB, name),
                                       Circle(x, y, radius, name) {
    if (radius > sideA || radius > sideB) {
        cout << "Error: radius is longer than side lengths" << endl;
    }
    assert(radius <= sideA && radius <= sideB);
}

CurveCut::CurveCut(const CurveCut &rhs) : Shape(rhs.origin.getx(), rhs.origin.gety(), rhs.shapeName),
                                          Square(rhs.origin.getx(), rhs.origin.gety(), rhs.side_a, rhs.shapeName),
                                          Rectangle(rhs.origin.getx(), rhs.origin.gety(), rhs.side_a, rhs.side_b,
                                                    rhs.shapeName),
                                          Circle(rhs.origin.getx(), rhs.origin.gety(), rhs.radius, rhs.shapeName) {
    if (radius > rhs.side_a || radius > rhs.side_b) {
        cout << "Error: radius is longer than side lengths" << endl;
    }
    assert(radius <= rhs.side_a && radius <= rhs.side_b);
}

CurveCut &CurveCut::operator=(const CurveCut &rhs) {
    if (this != &rhs) {
        setName(rhs.shapeName);
        origin.setx(rhs.origin.getx());
        origin.sety(rhs.origin.gety());
        side_a = rhs.side_a;
        side_b = rhs.side_b;
        radius = rhs.radius;
    }
    return *this;
}

void CurveCut::display() {
    cout << setprecision(2) << fixed;
    cout << "CurveCut Name: " << shapeName << endl;
    cout << "X-coordinate: " << origin.getx() << endl;
    cout << "Y-coordinate: " << origin.gety() << endl;
    cout << "Width: " << side_a << endl;
    cout << "Length: " << side_b << endl;
    cout << "Radius of the cut: " << radius << endl;
}

const double CurveCut::perimeter() const {
    return Rectangle::perimeter() - Circle::perimeter() / 4.f;
}

const double CurveCut::area() const {
    return Rectangle::area() - Circle::area() / 4.f;
}