#include "Library/Math/MathUtil.h"

#include <math/seadMathCalcCommon.h>

namespace al {
f32 normalize(f32 var, f32 min, f32 max) {
    if (sead::Mathf::abs(max - min) < 0.001f) {
        if (var < min)
            return 0.0f;
        else
            return 1.0f;
    }

    f32 clamped = sead::Mathf::clamp(var, min, max);
    return (clamped - min) / (max - min);
}

f32 normalize(s32 var, s32 min, s32 max) {
    s32 v3;
    f32 result;

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

        result = (static_cast<f32>(v3 - min) / static_cast<f32>(max - min));
    }

    return result;
}

f32 easeIn(f32 var) {
    return (((var * -0.5f) + 1.5f) * var) * var;
}

f32 easeOut(f32 var) {
    return (((var * -0.5f) * var) + 1.5f) * var;
}

f32 easeInOut(f32 var) {
    return (((var * -2.0f) + 3.0f) * var) * var;
}

f32 sign(f32 result) {
    f32 v1;
    v1 = result < 0.0f;
    if (result > 0.0f)
        result = 1.0f;
    if (v1)
        result = -1.0f;

    return result;
}

s32 sign(s32 var) {
    if (var < 0)
        return -1;
    if (var > 0)
        return 1;

    return 0;
}

f32 squareIn(f32 var) {
    return var * var;
}

f32 squareOut(f32 var) {
    return (2.0f - var) * var;
}

f32 powerIn(f32 x, f32 y) {
    return powf(x, y);
}

f32 powerOut(f32 x, f32 y) {
    return powf(x, 1.0 / y);
}

f32 lerpValue(f32 x, f32 y, f32 time) {
    if (time < 0.0f)
        time = 0.0f;
    else if (time > 1.0f)
        time = 1.0f;
    return (x * (1.0f - time)) + (time * y);
}

void calcParabolicFunctionParam(f32* gravity, f32* initialVelY, f32 maxHeight,
                                f32 verticalDistance) {
    // can be 0, 1, or -1
    f32 signOfA3 = maxHeight < 0.0f ? -1.0f : maxHeight > 0.0f ? 1.0f : maxHeight;

    f32 maxHeightAdjusted =
        sead::Mathf::sqrt(sead::Mathf::clampMin((maxHeight - verticalDistance) * maxHeight, 0.0));
    *initialVelY = 2 * ((signOfA3 * maxHeightAdjusted) + maxHeight);
    *gravity = verticalDistance - *initialVelY;
}

}  // namespace al
