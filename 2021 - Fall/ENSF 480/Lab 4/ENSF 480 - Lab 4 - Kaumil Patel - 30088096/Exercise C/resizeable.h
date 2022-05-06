//
// Created by spdmo on 10/20/2021.
//

#ifndef EXERCISE_C_RESIZEABLE_H
#define EXERCISE_C_RESIZEABLE_H

class Resizeable{
public:
    virtual void enlarge(int n) = 0;
    virtual void shrink(int n) = 0;
};

#endif //EXERCISE_C_RESIZEABLE_H

