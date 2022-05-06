/*
* File Name: point.cpp
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#include "cmath"
#include "iostream"
#include "iomanip"

using namespace std;

#include "point.h"

int Point::count = 0;

Point::Point(const double &x, const double &y) : x(x), y(y), id(1001 + count) {
    count++;
}

Point::~Point() {
    count--;
}

void Point::setx(const double &x) {
    this->x = x;
}

void Point::sety(const double &y) {
    this->y = y;
}

const int &Point::counter() {
    return count;
}

void Point::display() const {
    cout << setprecision(2) << fixed;
    cout << "X-coordinate: " << x << endl;
    cout << "Y-coordinate: " << y << endl;
}

const double Point::distance(const Point &other) const {
    return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

const double Point::distance(const Point &p1, const Point &p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

const double &Point::gety() const {
    return y;
}

const double &Point::getx() const {
    return x;
}

void Point::move(const double &dx, const double &dy) {
    x += dx;
    y += dy;
}

