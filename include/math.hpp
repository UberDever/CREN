//
// Created by uberdever on 14.02.2020.
//
#ifndef CREN_MATH_H
#define CREN_MATH_H

#include <cmath>

#include "util.hpp"

//math - all math, used in project (it can be not much, but it still required)
namespace math
{
    void calc_trig (float beta, float* s, float* c);

    util::e_exitCodes math_init();
}

#endif //CREN_MATH_H
