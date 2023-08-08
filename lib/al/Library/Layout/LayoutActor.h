#pragma once

#include "al/audio/AudioKeeper.h"
#include "al/camera/CameraDirector.h"
#include "al/effect/EffectKeeper.h"
#include "al/Library/HostIO/HioNode.h"
#include "al/layout/IUseLayout.h"
#include "al/message/IUseMessageSystem.h"
#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Scene/SceneObjHolder.h"

namespace al {

class IUseLayoutAction {};

class LayoutActor : public al::IUseHioNode,
                    public al::IUseNerve,
                    public al::IUseLayout,
                    public al::IUseLayoutAction,
                    public al::IUseMessageSystem,
                    public al::IUseCamera,
                    public al::IUseAudioKeeper,
                    public al::IUseEffectKeeper,
                    public al::IUseSceneObjHolder {};
}  // namespace al
