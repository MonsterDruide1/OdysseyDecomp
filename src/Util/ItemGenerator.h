#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Util/ItemUtil.h"

namespace al {
class ActorInitInfo;
class LiveActor;
}  // namespace al
class Shine;
class KuriboMini;

class ItemGenerator {
public:
    ItemGenerator();
    ItemGenerator(al::LiveActor*, const al::ActorInitInfo&);

    void initLinkShine(al::LiveActor*, const al::ActorInitInfo&, bool);
    void initNoLinkShine(al::LiveActor*, const al::ActorInitInfo&, bool);
    void initHintPhotoShine(al::LiveActor*, const al::ActorInitInfo&);
    void createShineEffectInsideObject(const al::ActorInitInfo&);
    bool tryUpdateHintTransIfExistShine();
    bool tryUpdateHintTransIfExistShine(const sead::Vector3f&);
    bool isEnableGenerateByCount(s32) const;
    void generate(const sead::Vector3f&, const sead::Quatf&);
    void generate(const sead::Vector3f&, const sead::Vector3f&);
    bool tryGenerate(const sead::Vector3f&, const sead::Quatf&, s32);
    bool tryGenerate(const sead::Vector3f&, const sead::Vector3f&, s32);
    bool isNone() const;
    bool isShine() const;
    bool isLifeUp() const;
    bool isLifeMaxUp() const;
    bool isCoin() const;
    bool isCoinBlow() const;
    bool isCoinStackBound() const;
    bool isKuriboMini3() const;
    bool isKuriboMini8() const;

private:
    al::LiveActor* mCreator = nullptr;
    Shine* mLinkShine = nullptr;
    KuriboMini** mKuriboMiniArray = nullptr;
    s32 mKuriboMiniCount = 0;
    s32 mItemType = -1;  // mismatches as rs::ItemType despite it being that
    s32 field_20 = 0;
};
