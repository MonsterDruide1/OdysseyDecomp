#pragma once

#include <basis/seadTypes.h>

namespace al {

class FractalGenerator {
public:
    FractalGenerator(u32 permutations, f32 amplitude, f32 scale, f32 nextOrderAmplitude);

    void setParam(u32 permutations, f32 amplitude, f32 scale, f32 nextOrderAmplitude);
    f32 calcFractal(f32 x, f32 y, bool useSmoothPerlingNoise);
    f32 makeSmoothPerlinNoise(f32 x, f32 y);
    f32 makePerlinNoise(f32 x, f32 y);
    f32 calcMultiFractal(f32 x, f32 y, f32 baseAmplitude, bool useSmoothPerlingNoise);
    f32 makeRandom(s32 x, s32 y);
    f32 makeSmoothRandom(s32 x, s32 y);

private:
    u32 mPermutations;
    f32 mAmplitude;
    f32 mScale;
    f32 mNextOrderAmplitude;
};

}  // namespace al
