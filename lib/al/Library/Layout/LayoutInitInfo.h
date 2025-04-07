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

void initLayoutActor(LayoutActor* layoutActor, const LayoutInitInfo& info, const char* archiveName,
                     const char* suffix = nullptr);
void initLayoutActorLocalized(LayoutActor* layoutActor, const LayoutInitInfo& info,
                              const char* archiveName, const char* suffix = nullptr);
void initLayoutActorUseOtherMessage(LayoutActor* layoutActor, const LayoutInitInfo& info,
                                    const char* archiveName, const char* suffix,
                                    const char* messageArchiveName);
void initLayoutTextPaneAnimator(LayoutActor* layoutActor, const char* archiveName);
void initLayoutTextPaneAnimatorWithShadow(LayoutActor* layoutActor, const char* archiveName);
void initLayoutPartsActor(LayoutActor* partsActor, LayoutActor* parentActor,
                          const LayoutInitInfo& info, const char* archiveName,
                          const char* suffix = nullptr);
void initLayoutPartsActorLocalized(LayoutActor* partsActor, LayoutActor* parentActor,
                                   const LayoutInitInfo& info, const char* archiveName,
                                   const char* suffix = nullptr);

}  // namespace al
