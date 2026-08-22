#include "Library/Model/SkyDirector.h"

#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ParameterBase.h"

namespace al {

// NON-MATCHING: Stack mismatch https://decomp.me/scratch/uG8AT
SkyParam::SkyParam() {
    mParameterObj = new ParameterObj();

    mSkyName = new ParameterString64(StringTmp<64>("RSGraphicTestSkyBlue"), mParameterObj, "Name",
                                     "空のモデル名", "", true);

    mRotate = new ParameterV3f({0.0f, 0.0f, 0.0f}, mParameterObj, "Rotate", "回転",
                               "Min=-360.0f, Max=360.0f", true);

    mStarIntensity = new ParameterF32(0.0f, mParameterObj, "StarIntensity", "星の明るさ",
                                      "Min=0.0f, Max=100.0f", true);
}

bool SkyParam::isEqual(const IUseRequestParam& other) const {
    const SkyParam& param = static_cast<const SkyParam&>(other);

    return isNear(getRotate(), param.getRotate()) &&
           isEqualString(getSkyName(), param.getSkyName()) &&
           isNear(getStarIntensity(), param.getStarIntensity());
}

const sead::Vector3f& SkyParam::getRotate() const {
    return mRotate->getValue();
}

const char* SkyParam::getSkyName() const {
    return mSkyName->getValue().cstr();
}

f32 SkyParam::getStarIntensity() const {
    return mStarIntensity->getValue();
}

}  // namespace al
