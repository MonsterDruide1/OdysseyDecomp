#include "Library/Math/MathUtil.h"
#include <cmath>
#include <math/seadMathCalcCommon.h>

namespace al {
float normalize(float var, float min, float max) {
    if (sead::Mathf::abs(max - min) < 0.001f) {
        if (var < min)
            return 0.0f;
        else
            return 1.0f;
    }

    float clamped = sead::Mathf::clamp(var, min, max);
    return (clamped - min) / (max - min);
}

float normalize(signed int var, signed int min, signed int max) {
    signed int v3;
    float result;

    if (var <= min)
        return 0.0f;

    if (max - min <= 0) {
        result = 1.0f;

        if (var < min)
            result = 0.0f;
    } else {
        if (var <= max)
            v3 = var;
        else
            v3 = max;

        if (var < min)
            v3 = min;

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

float sign(float result) {
    float v1;
    v1 = result < 0.0f;
    if (result > 0.0f) {
        result = 1.0f;
    }
    if (v1) {
        result = -1.0f;
    }

    return result;
}

int sign(int var) {
    if (var < 0) {
        return -1;
    }
    if (var > 0) {
        return 1;
    }

    return 0;
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

float lerpValue(float x, float y, float time) {
    if (time < 0.0f)
        time = 0.0f;
    else if (time > 1.0f)
        time = 1.0f;
    return (x * (1.0f - time)) + (time * y);
}
};  // namespace al
