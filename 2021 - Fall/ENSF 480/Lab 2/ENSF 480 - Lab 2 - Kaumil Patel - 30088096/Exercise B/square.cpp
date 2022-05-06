/*
* File Name: square.cpp
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#include "iostream"
#include "iomanip"

using namespace std;

#include "square.h"

Square::Square(const double &x, const double &y, const double &sideLength, const char *name) : Shape(x, y, name),
                                                                                            side_a(sideLength) {}

void Square::display() {
    cout << setprecision(2) << fixed;
    cout << "Square Name: " << shapeName << endl;
    cout << "X-coordinate: " << origin.getx() << endl;
    cout << "Y-coordinate: " << origin.gety() << endl;
    cout << "Side a: " << side_a << endl;
    cout << "Area: " << area() << endl;
    cout << "Perimeter: " << perimeter() << endl;
}

const double  Square::perimeter() const{
    return 4 * side_a;
}

const double  Square::area()const {
    return side_a * side_a;
}

//double Square::distance(Square &other) {
//    Point p1(origin.getx() + side_a / 2, origin.gety() + side_a / 2);
//    Point p2(other.origin.getx() + other.side_a / 2, other.origin.gety() + other.side_a / 2);
//    return p1.distance(p2);
//}
//
//double Square::distance(Square &the_shape, Square &other) {
//    Point p1(the_shape.origin.getx() + the_shape.side_a / 2, the_shape.origin.gety() + the_shape.side_a / 2);
//    Point p2(other.origin.getx() + other.side_a / 2, other.origin.gety() + other.side_a / 2);
//    return p1.distance(p2);
//}
