/*
* File Name: human.h
* Assignment: Lab 1 Exercise D
* Completed by: Kaumil Patel
* Submission Date: Sept 23, 2020
*/

#ifndef LAB_1_HUMAN_H
#define LAB_1_HUMAN_H


#include "point.h"

class Human {
protected:
    Point location;   // Location of an object of Human on a Cartisian Plain
    char *name;       // Human's name
public:
    Human(const char *nam = "", const double &x = 0, const double &y = 0);
    ~Human();
    const char* get_name();
    void set_name(const char* name);
    const Point& get_point()const;
    virtual void display();
};


#endif //LAB_1_HUMAN_H
