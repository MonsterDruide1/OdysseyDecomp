#pragma once

#include "al/layout/IUseLayout.h"
#include "al/hio/HioNode.h"
#include "al/nerve/Nerve.h"
#include "al/camera/CameraDirector.h"
#include "al/audio/AudioKeeper.h"
#include "al/effect/EffectKeeper.h"
#include "al/scene/SceneObjHolder.h"
#include "al/message/IUseMessageSystem.h"

namespace al {

    class IUseLayoutAction {

    };

    class LayoutActor : public al::IUseHioNode, public al::IUseNerve, public al::IUseLayout, public al::IUseLayoutAction, public al::IUseMessageSystem, public al::IUseCamera, public al::IUseAudioKeeper, public al::IUseEffectKeeper, public al::IUseSceneObjHolder {
        
    };
}