#pragma once

namespace al
{
    float easeIn(float);
    float easeOut(float);
    float easeInOut(float);

    float sign(float);
    bool sign(int);

    float squareIn(float);
    float squareOut(float);

    float powerIn(float, float);
    float powerOut(float, float);

    float normalize(float, float, float);
    float normalize(signed int, signed int, signed int);
};