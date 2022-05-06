/*
* File Name: point.h
* Assignment: Lab 1 Exercise D
* Completed by: Kaumil Patel
* Submission Date: Sept 23, 2020
*/

#ifndef LAB_1_POINT_H
#define LAB_1_POINT_H

class Point{
private:
    double x;     // x coordinate of a location on Cartisian Plain
    double y;     // y coordinate of a location on Cartisian Plain
    friend class Human;
public:
    Point(const double& a = 0, const double& b = 0);
    const double& get_x() const;
    const double& get_y() const;
    void set_x(const double& a);
    void set_y(const double& a);
};


#endif //LAB_1_POINT_H
