#include "al/util/MathUtil.h"
#include <cmath>

namespace al
{
    float normalize(float var, float min, float max)
    {
    float v4;
    bool v5;
    float result;
    float v7;

    v4 = -(max - min);
    if ( (max - min) > 0.0f )
        v4 = max - min;
    if ( v4 >= 0.001f )
    {
        v7 = min;
        if ( var >= min )
        {
        v7 = var;
        if ( var > max )
            v7 = max;
        }
        result = (v7 - min) / (max - min);
    }
    else
    {
        v5 = var < min;
        result = 1.0f;
        if ( v5 )
        result = 0.0f;
    }
    return result;
    }


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

    float easeIn(float var) {
        return (((var * -0.5f) + 1.5f) * var) * var;
    }
    float easeOut(float var) {
        return (((var * -0.5f) * var) + 1.5f) * var;
    }
    float easeInOut(float var) {
        return (((var * -2.0f) + 3.0f) * var) * var;
    }

    float squareIn(float var) {
        return var * var;
    }
    float squareOut(float var) {
        return (2.0f - var) * var;
    }

    float powerIn(float x, float y) {
        return powf(x, y);
    }
    float powerOut(float x, float y) {
        return powf(x, 1.0 / y);
    }
};