/*
* File Name: Employee.h
* Assignment: Lab 1 Exercise C
* Completed by: Kaumil Patel
* Submission Date: Sept 23, 2020
*/

#ifndef EXERCISE_C_EMPLOYEE_H
#define EXERCISE_C_EMPLOYEE_H

#include <string>

#include "Date.h"
#include "Address.h"
#include "Name.h"


class Employee {
private:
    Name name;
    Address address;
    Date birthday;
    std::string employeeState;
public:
};


#endif //EXERCISE_C_EMPLOYEE_H
