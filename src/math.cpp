//
// Created by uberdever on 19.02.2020.
//

#include <util.hpp>
#include "math.h"

namespace math
{
    constexpr float PI = 3.14159265;

    float sin[360] = {};
    float cos[360] = {};

    void calc_trig(float beta, float* s, float* c)
    {
        constexpr int32_t IT_NUM = 25;
        constexpr int32_t ANG_LEN = 13;

        float angle;
        float angles[ANG_LEN] = {
                7.85398163E-01,
                4.63647609E-01,
                2.44978663E-01,
                1.24354994E-01,
                6.24188099E-02,
                3.12398334E-02,
                1.56237286E-02,
                7.81234106E-03,
                3.90623013E-03,
                1.95312251E-03,
                9.76562189E-04,
                4.88281211E-04,
                2.44140620E-04};


        float kprod = 0.60725293;

        float factor;
        float sign_factor = 1;
        float two_factor;
        float sigma;
        float& sine = *s;
        float& cosine = *c;
        int32_t j;

        beta = beta * PI / 180;

        sine = 0;
        cosine = 1.0;

        two_factor = 1.0;
        angle = angles[0];

        for (j = 1; j <= IT_NUM; j++ )
        {
            if ( beta < 0.0 )
            {
                sigma = -1.0;
            }
            else
            {
                sigma = 1.0;
            }

            factor = sigma * two_factor;

            float new_cos =          cosine - factor * sine;
            float new_sin = factor * cosine +          sine;

            cosine = new_cos;
            sine = new_sin;

            beta = beta - sigma * angle;

            two_factor /= 2.0;

            if ( ANG_LEN < j + 1 )
            {
                angle = angle / 2.0;
            }
            else
            {
                angle = angles[j];
            }
        }


        cosine = cosine * kprod;
        sine = sine * kprod;

        cosine *= sign_factor;
        sine *= sign_factor;

        return;
    }

    util::e_exitCodes math_init()
    {
        for (int32_t i = 0; i < 90; i++)
        {
            float s, c;
            calc_trig(i, &s, &c);
            sin[i] = s;
            cos[i] = c;
            sin[i + 90] = c;
            cos[i + 90] = -s;
            sin[i + 180] = -s;
            cos[i + 180] = -c;
            sin[i + 270] = -c;
            cos[i + 270] = s;
        }

        return util::e_exitCodes::OK;
    }
}