#pragma once

#include <container/seadPtrArray.h>
#include <gfx/seadColor.h>

namespace al {
class PrePassLightBase;
class ParameterObj;
class ParameterIo;
class ParameterBool;
class ParameterStringRef;
class ParameterC4f;
class ParameterArray;
class LiveActor;
class Resource;
class ActorInitInfo;

class ActorPrePassLightKeeper {
    class UserColor {
    public:
        UserColor();

    private:
        ParameterObj* mList;
        ParameterStringRef* mName;
        ParameterC4f* mColor;
    };
    static_assert(sizeof(UserColor) == 0x18);

public:
    static bool isExistFile(const Resource*, const char*);

    ActorPrePassLightKeeper(LiveActor*);
    void init(const Resource*, const ActorInitInfo&, const char*);
    void initLightNum(s32);
    void initAfterPlacement();
    void appear(bool);
    void requestKill();
    void hideModel();
    void updateHideModel(bool);
    PrePassLightBase* getLightBase(const char*);
    PrePassLightBase* getLightBase(s32);
    sead::Color4f* findUserColor(const char*);

private:
    ParameterIo* mParameterIo;
    ParameterBool* mIsIgnoreHideModel;
    ParameterArray* mLightParameters;
    ParameterArray* mUserColorParameters;
    sead::PtrArray<PrePassLightBase*> mLights;
    sead::PtrArray<UserColor*> mUserColors;
    LiveActor* mLiveActor;
    bool mIsIgnorePrePassLightYaml;
};
static_assert(sizeof(ActorPrePassLightKeeper) == 0x50);
}  // namespace al
