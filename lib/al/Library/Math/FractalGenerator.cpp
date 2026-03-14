#include "Library/Math/FractalGenerator.h"

#include "Library/Math/MathUtil.h"

namespace al {
FractalGenerator::FractalGenerator(u32 permutations, f32 amplitude, f32 scale,
                                   f32 nextOrdeAmplitude)
    : mPermutations(permutations), mAmplitude(amplitude), mScale(scale),
      mNextOrdeAmplitude(nextOrdeAmplitude) {}

void FractalGenerator::setParam(u32 permutations, f32 amplitude, f32 scale, f32 nextOrdeAmplitude) {
    mPermutations = permutations;
    mAmplitude = amplitude;
    mScale = scale;
    mNextOrdeAmplitude = nextOrdeAmplitude;
}

f32 FractalGenerator::calcFractal(f32 x, f32 y, bool useSmoothPerlingNoise) {
    f32 value = 0.0f;

    f32 amplitude = mAmplitude;
    f32 scale = mScale;
    for (s32 i = 0; i < (s32)(mPermutations - 1); i++) {
        if (useSmoothPerlingNoise)
            value += makeSmoothPerlinNoise(scale * x, scale * y) * amplitude;
        else
            value += makePerlinNoise(scale * x, scale * y) * amplitude;

        scale *= 2;
        amplitude *= mNextOrdeAmplitude;
    }

    return value;
}

f32 FractalGenerator::makeSmoothPerlinNoise(f32 x, f32 y) {
    s32 gridX = x;
    s32 gridY = y;
    f32 fracX = x - gridX;
    f32 fracY = y - gridY;

    f32 r00 = makeSmoothRandom(gridX, gridY);
    f32 r10 = makeSmoothRandom(gridX + 1, gridY);
    f32 r01 = makeSmoothRandom(gridX, gridY + 1);
    f32 r11 = makeSmoothRandom(gridX + 1, gridY + 1);

    return lerpValue(lerpValue(r00, r10, easeInOut(fracX)), lerpValue(r01, r11, easeInOut(fracX)),
                     easeInOut(fracY));
}

f32 FractalGenerator::makePerlinNoise(f32 x, f32 y) {
    s32 gridX = x;
    s32 gridY = y;
    f32 fracX = x - gridX;
    f32 fracY = y - gridY;

    f32 r00 = makeRandom(gridX, gridY);
    f32 r10 = makeRandom(gridX + 1, gridY);
    f32 r01 = makeRandom(gridX, gridY + 1);
    f32 r11 = makeRandom(gridX + 1, gridY + 1);

    return lerpValue(lerpValue(r00, r10, easeInOut(fracX)), lerpValue(r01, r11, easeInOut(fracX)),
                     easeInOut(fracY));
}

f32 FractalGenerator::calcMultiFractal(f32 x, f32 y, f32 baseAmplitude,
                                       bool useSmoothPerlingNoise) {
    f32 value = 1.0f;

    f32 amplitude = mAmplitude;
    f32 scale = mScale;
    for (s32 i = 0; i < (s32)(mPermutations - 1); i++) {
        if (useSmoothPerlingNoise)
            value *= baseAmplitude * makeSmoothPerlinNoise(scale * x, scale * y) * amplitude;
        else
            value *= baseAmplitude * makePerlinNoise(scale * x, scale * y) * amplitude;

        scale *= 2;
        amplitude *= mNextOrdeAmplitude;
    }

    return value;
}

inline s32 makeSeed(s32 x, s32 y) {
    s32 seed = x + y * 57;
    seed ^= seed << 13;
    return seed;
}

inline f32 makeNoise(s32 seed) {
    // NOTE: The only true random are all these magic values
    s32 hash = (seed * seed * 0x4bd9 + 0x2e59b) * seed + 0x4bcb4b;
    return static_cast<f32>(hash & 0x4b746f) / -3354521.0f;
}

f32 FractalGenerator::makeRandom(s32 x, s32 y) {
    return makeNoise(makeSeed(x, y)) + 1.0f;
}

f32 FractalGenerator::makeSmoothRandom(s32 x, s32 y) {
    f32 corners = (makeRandom(x - 1, y - 1) + makeRandom(x + 1, y - 1) + makeRandom(x - 1, y + 1) +
                   makeRandom(x + 1, y + 1)) /
                  16;
    f32 sides = (makeRandom(x - 1, y) + makeRandom(x + 1, y) + makeRandom(x, y - 1) +
                 makeRandom(x, y + 1)) /
                8;
    f32 center = makeRandom(x, y) / 4;
    return corners + sides + center;
}

}  // namespace al
