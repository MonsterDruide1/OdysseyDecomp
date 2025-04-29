#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

namespace al {
struct ActorInitInfo;
class SkyParam;
class Sky;
class ParamRequestInterp;

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
