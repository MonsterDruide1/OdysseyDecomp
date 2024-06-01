#pragma once

#include "Library/Layout/LayoutSceneInfo.h"

namespace al {
class ExecuteDirector;
class EffectSystemInfo;
class AudioDirector;
class LayoutSystem;
class LayoutActor;

class LayoutInitInfo : public LayoutSceneInfo {
public:
    void init(ExecuteDirector*, const EffectSystemInfo*, SceneObjHolder*, const AudioDirector*,
              CameraDirector*, const LayoutSystem*, const MessageSystem*, const GamePadSystem*,
              PadRumbleDirector*);

    MessageSystem* getMessageSystem() const;

private:
    void* field_30;
    void* field_38;
    void* field_40;
    ExecuteDirector* mExecuteDirector;
    EffectSystemInfo* mEffectSysInfo;
    AudioDirector* mAudioDirector;
    LayoutSystem* mLayoutSystem;
};

void initLayoutActor(LayoutActor*, const LayoutInitInfo&, const char*, const char*);
void initLayoutActorLocalized(LayoutActor*, const LayoutInitInfo&, const char*, const char*);
void initLayoutActorUseOtherMessage(LayoutActor*, const LayoutInitInfo&, const char*, const char*,
                                    const char*);
void initLayoutTextPaneAnimator(LayoutActor*, const char*);
void initLayoutTextPaneAnimatorWithShadow(LayoutActor*, const char*);
void initLayoutPartsActor(LayoutActor*, LayoutActor*, const LayoutInitInfo&, const char*,
                          const char*);
void initLayoutPartsActorLocalized(LayoutActor*, LayoutActor*, const LayoutInitInfo&, const char*,
                                   const char*);

}  // namespace al
