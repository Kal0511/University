/*
* File Name: rectangle.cpp
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#include "iostream"
#include "iomanip"

using namespace std;

#include "rectangle.h"

Rectangle::Rectangle(const double &x, const double &y, const double &sideA, const double &sideB, const char* name) : Square(x,y,sideA,name), side_b(sideB) {}

void Rectangle::set_side_a(const double &a) {
    side_a = a;

}

void Rectangle::set_side_b(const double &b) {
    side_b = b;
}

void Rectangle::display() {
    cout << setprecision(2) << fixed;
    cout << "Rectangle Name: " << shapeName << endl;
    cout << "X-coordinate: " << origin.getx() << endl;
    cout << "Y-coordinate: " << origin.gety() << endl;
    cout << "Side a: " << side_a << endl;
    cout << "Side b: " << side_b << endl;
    cout << "Area: " << area() << endl;
    cout << "Perimeter: " << perimeter() << endl;
}

const double Rectangle::perimeter() const{
    return 2 * side_a + 2 * side_b;
}

const double Rectangle::area() const{
    return side_a * side_b;
}

//double Rectangle::distance(Rectangle &other) {
//    Point p1(origin.getx() + side_a / 2, origin.gety() + side_b / 2);
//    Point p2(other.origin.getx() + other.side_a / 2, other.origin.gety() + other.side_b / 2);
//    return p1.distance(p2);
//}
//
//double Rectangle::distance(Rectangle &the_shape, Rectangle &other) {
//    Point p1(the_shape.origin.getx() + the_shape.side_a / 2, the_shape.origin.gety() + the_shape.side_b / 2);
//    Point p2(other.origin.getx() + other.side_a / 2, other.origin.gety() + other.side_b / 2);
//    return p1.distance(p2);
//}
