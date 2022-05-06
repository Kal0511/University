/*
* File Name: human.cpp
* Assignment: Lab 1 Exercise D
* Completed by: Kaumil Patel
* Submission Date: Sept 23, 2020
*/

#include "human.h"
#include <cstring>
#include <iostream>

using namespace std;

Human::Human(const char *nam, const double &x, const double &y) : name(new char[strlen(nam) + 1]), location(x, y) {
    strcpy(this->name, nam);
}

Human::~Human() {
    delete[] name;
}

const char* Human::get_name() {
    return name;
}

void Human::set_name(const char* name) {
    delete[] this->name;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

const Point& Human::get_point() const {
    return location;
}

void Human::display() {
    cout << "Human Name: " << name << "\nHuman Location: "
         << location.x << " ,"
         << location.y << ".\n" << endl;
}