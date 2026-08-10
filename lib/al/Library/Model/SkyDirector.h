#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Project/Base/ParamRequestInterp.h"

namespace al {
struct ActorInitInfo;
class SkyParam;
class Sky;
class ParamRequestInterp;
class ParameterObj;
class ParameterString64;
class ParameterV3f;
class ParameterF32;

class SkyParam : public IUseRequestParam {
public:
    SkyParam();

    const char* getParamName() const override { return "空"; }

    ParameterObj* getParamObj() override { return mParameterObj; }

    const ParameterObj* getParamObj() const override { return mParameterObj; }

    bool isEqual(const IUseRequestParam& other) const override;

    const sead::Vector3f& getRotate() const;
    const char* getSkyName() const;
    f32 getStarIntensity() const;

private:
    ParameterObj* mParameterObj;
    ParameterString64* mSkyName;
    ParameterV3f* mRotate;
    ParameterF32* mStarIntensity;
};

static_assert(sizeof(SkyParam) == 0x28);

class SkyDirector {
public:
    SkyDirector();

    void initProjectResource();
    void init(const ActorInitInfo&);
    void endInit();
    void clearRequest();
    void update();
    SkyParam* getCurrentParam() const;
    Sky* tryGetSky(const char*) const;
    bool tryRegistAndCreateSky(const char*);
    bool requestParam(s32, s32, const SkyParam&);
    f32 getCurrentStarIntensity() const;
    Sky* tryGetCurrentSky() const;

private:
    ParamRequestInterp* mParamRequestInterp;
    void* _8[1];
    sead::PtrArray<Sky> _10;
    void* _20[2];
    sead::FixedSafeString<64> _30;
};

static_assert(sizeof(SkyDirector) == 0x88);

}  // namespace al
