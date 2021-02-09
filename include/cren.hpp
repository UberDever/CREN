//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_CREN_H
#define CREN_CREN_H

#include "s_exit.hpp"
#include "s_temp.hpp"
#include "s_gameplay.hpp"
#include "s_UI.hpp"

namespace cren
{
    using namespace util;

    e_exitCodes exec();
    e_exitCodes init();
    void clean();

    void s_init();
}

#endif //CREN_CREN_H
