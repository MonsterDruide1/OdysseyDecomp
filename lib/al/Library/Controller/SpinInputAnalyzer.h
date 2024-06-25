#pragma once

#include <container/seadBuffer.h>
#include <math/seadVector.h>

namespace al {

class SpinInputAnalyzer {
public:
    SpinInputAnalyzer();
    void update(const sead::Vector2f& controllerInput);

public:
    sead::Buffer<f32> mChangeHistory;
    sead::Buffer<sead::Vector2f> mInputHistory;
    s32 mNumUsableEntries = 0;
    s32 mCurrentIndex = 0;
    s32 mSpinDirection = 0;
    s32 mSpinTimer = 0;
};

}  // namespace al
