#pragma once

#include "Library/Layout/LayoutSceneInfo.h"

namespace agl {
class DrawContext;
}

namespace nn::ui2d {
class DrawInfo;
}

namespace al {
class ExecuteDirector;
class EffectSystemInfo;
class AudioDirector;
class LayoutSystem;
class LayoutActor;
class OcclusionCullingJudge;

class LayoutInitInfo : public LayoutSceneInfo {
public:
    void init(ExecuteDirector*, const EffectSystemInfo*, SceneObjHolder*, const AudioDirector*,
              CameraDirector*, const LayoutSystem*, const MessageSystem*, const GamePadSystem*,
              PadRumbleDirector*);

    MessageSystem* getMessageSystem() const;

    void setDrawContext(agl::DrawContext* drawContext) { mDrawContext = drawContext; }

    void setDrawInfo(nn::ui2d::DrawInfo* drawInfo) { mDrawInfo = drawInfo; }

    void setOcclusionCullingJudge(OcclusionCullingJudge* judge) { mOcclusionCullingJudge = judge; }

private:
    agl::DrawContext* mDrawContext;
    nn::ui2d::DrawInfo* mDrawInfo;
    OcclusionCullingJudge* mOcclusionCullingJudge;
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
