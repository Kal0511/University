/*
* File Name: shape.cpp
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#include "cstring"
#include "iostream"
#include "iomanip"

using namespace std;

#include "shape.h"

Shape::Shape(const double &x, const double &y, const char *name) : origin(x, y), shapeName(new char[strlen(name) + 1]) {
    strcpy(this->shapeName, name);
}

Shape::~Shape() {
    delete[] shapeName;
}

Shape &Shape::operator=(const Shape &other) {
    if (this != &other) {
        origin = other.origin;
        delete[] shapeName;
        shapeName = new char[strlen(other.shapeName) + 1];
        strcpy(shapeName, other.shapeName);
    }
    return *this;
}

void Shape::display() {
    cout << setprecision(2) << fixed;
    cout << "Shape Name: " << shapeName << endl;
    cout << "X-coordinate: " << origin.getx() << endl;
    cout << "Y-coordinate: " << origin.gety() << endl;
}

const double Shape::distance(Shape &other) {
    return this->origin.distance(other.origin);
}

const double Shape::distance(Shape &the_shape, Shape &other) {
    return the_shape.origin.distance(other.origin);
}

const Point &Shape::getOrigin() {
    return origin;
}

char *Shape::getName() {
    return shapeName;
}

void Shape::move(const double& dx, const double& dy) {
    origin.x += dx;
    origin.y += dy;
}
