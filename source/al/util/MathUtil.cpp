#include "al/util/MathUtil.h"

namespace al
{
    float normalize(signed int var, signed int min, signed int max)
    {
        signed int v3;
        float result;

        if (var <= min)
        {
            return 0.0f;
        }

        if (max - min <= 0)
        {
            result = 1.0f;
            
            if (var < min)
            {
                result = 0.0f;
            }
        }
        else
        {
            if (var <= max)
            {
                v3 = var;
            }
            else
            {
                v3 = max;
            }

            if (var < min)
            {
                v3 = min;
            }

            result = (static_cast<float>(v3 - min) / static_cast<float>(max - min));
        }

        return result;
    }
};