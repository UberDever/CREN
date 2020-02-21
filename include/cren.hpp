//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_CREN_H
#define CREN_CREN_H

#include "math.hpp"
#include "util.hpp"
#include "gut.hpp"
#include "res.hpp"

#include "s_exit.hpp"
#include "s_mainmenu.hpp"
#include "s_gameplay.hpp"

namespace cren
{
    //namespaces
    using namespace math;
    using namespace gut;
    using namespace util;
    using namespace res;

    //defined vars
    constexpr int SCENE_NUM = 3;

    //main block (declaration)
    e_exitCodes exec();

    e_exitCodes init();
    void clean();

    void s_init();
}

#endif //CREN_CREN_H
