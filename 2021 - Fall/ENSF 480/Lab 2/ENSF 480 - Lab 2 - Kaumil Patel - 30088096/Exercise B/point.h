/*
* File Name: point.h
* Assignment: Lab 2 Exercise B
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Sept 30, 2021
*/

#ifndef EXERCISE_B_Point_H
#define EXERCISE_B_Point_H

class Point {
public:
    friend class Shape;

    Point(const double &x, const double &y);

    ~Point();

    void setx(const double &x);

    void sety(const double &y);

    const double &getx() const;

    const double &gety() const;

    void display() const;

    static const int &counter();

    const double distance(const Point &other);

    static const double distance(const Point &p1, const Point &p2);

    void move(const double &dx, const double &dy);

protected:
    double x;
    double y;
    int id;
    static int count;
};


#endif //EXERCISE_B_Point_H
