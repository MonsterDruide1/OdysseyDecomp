#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseAudioKeeper;
}  // namespace al

class CollectBgmPlayer : public al::HioNode, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CollectBgmPlayer;

    CollectBgmPlayer();

    void init(const al::IUseAudioKeeper*);

    bool isPlaying(const char*, const char*) const;

    void prepare();
    void start(const char* bgmName, const char* bgmSituation);
    void stop(s32);

private:
    const al::IUseAudioKeeper* mAudioKeeper = nullptr;
    const char* mCurBgmName = nullptr;
    const char* mCurBgmSituation = nullptr;
};

static_assert(sizeof(CollectBgmPlayer) == 0x20);
