#include "Library/Controller/SpinInputAnalyzer.h"

#include "Library/Math/MathLengthUtil.h"

namespace al {

SpinInputAnalyzer::SpinInputAnalyzer() {
    mChangeHistory.allocBufferAssert(128, nullptr);
    mChangeHistory.fill(0.0f);

    mInputHistory.allocBufferAssert(128, nullptr);
    mInputHistory.fill(sead::Vector2f::zero);
}

void SpinInputAnalyzer::update(const sead::Vector2f& controllerInput) {
    s32 prevIndex = (mCurrentIndex - 1 + 128) % 128;

    mInputHistory[mCurrentIndex].set(controllerInput);
    mChangeHistory[mCurrentIndex] =
        mInputHistory[prevIndex].cross(mInputHistory[mCurrentIndex]) * 0.5f;

    sead::Vector2f currentInput = mInputHistory[mCurrentIndex];
    tryNormalizeOrZero(&currentInput);

    f32 sumOfChanges = 0.0f;
    for (s32 i = 0; (u32)i < 70 && i < mNumUsableEntries; i++) {
        s32 indexMinusI = (mCurrentIndex + (128 - i)) % 128;

        sead::Vector2f iPrevInput = mInputHistory[indexMinusI];
        tryNormalizeOrZero(&iPrevInput);
        if ((u32)i >= 30) {
            if (sead::Mathf::abs(sumOfChanges) >= 5.0f && iPrevInput.dot(currentInput) > 0.7f) {
                f32 spinDirection = sign(sumOfChanges);
                mNumUsableEntries = 0;
                mSpinDirection = (s32)spinDirection;
                mSpinTimer = 31;  // actually, it ends up being 30 due to subtracting one below
                break;
            }
        }

        sumOfChanges += mChangeHistory[indexMinusI];
    }

    if (mSpinTimer >= 1) {
        mSpinTimer--;
        if (mSpinTimer == 0)
            mSpinDirection = 0;
    }

    mCurrentIndex = (mCurrentIndex + 1) % 128;
    if (mNumUsableEntries <= 127)
        mNumUsableEntries++;
}

}  // namespace al
