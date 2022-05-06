/*
* File Name: Customer.h
* Assignment: Lab 1 Exercise C
* Completed by: Kaumil Patel
* Submission Date: Sept 23, 2020
*/

#ifndef EXERCISE_C_CUSTOMER_H
#define EXERCISE_C_CUSTOMER_H

#include <string>

#include "Date.h"
#include "Address.h"
#include "Name.h"

class Customer {
private:
    Name name;
    Address address;
    std::string phone;
public:
};


#endif //EXERCISE_C_CUSTOMER_H
