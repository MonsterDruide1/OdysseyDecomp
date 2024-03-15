#include "Library/Action/ActorActionKeeper.h"

namespace al {
ActorActionKeeper::ActorActionKeeper(LiveActor* parentActor, char const* actorName,
                                     ActionAnimCtrl* animCtrl, NerveActionCtrl* nrvActionCtrl,
                                     ActionFlagCtrl* flagCtrl, ActionEffectCtrl* effectCtrl,
                                     ActionSeCtrl* seCtrl, ActionBgmCtrl* bgmCtrl,
                                     ActionPadAndCameraCtrl* padAndCamCtrl,
                                     ActionScreenEffectCtrl* screenEffectCtrl)
    : mParentActor(parentActor), mActorName(actorName), mAnimCtrl(animCtrl),
      mNrvActionCtrl(nrvActionCtrl), mFlagCtrl(flagCtrl), mEffectCtrl(effectCtrl), mSeCtrl(seCtrl),
      mBgmCtrl(bgmCtrl), mPadAndCamCtrl(padAndCamCtrl), mScreenEffectCtrl(screenEffectCtrl) {}

}  // namespace al
