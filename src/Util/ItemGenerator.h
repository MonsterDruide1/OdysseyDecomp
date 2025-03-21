#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ActorInitInfo;
class LiveActor;
}  // namespace al
class Shine;
class KuriboMini;

class ItemGenerator {
public:
    ItemGenerator();
    ItemGenerator(al::LiveActor* creator, const al::ActorInitInfo& info);

    void initLinkShine(al::LiveActor* creator, const al::ActorInitInfo& info, bool a3);
    void initNoLinkShine(al::LiveActor* creator, const al::ActorInitInfo& info, bool a3);
    void initHintPhotoShine(al::LiveActor* creator, const al::ActorInitInfo& info);
    void createShineEffectInsideObject(const al::ActorInitInfo& info);
    bool tryUpdateHintTransIfExistShine();
    bool tryUpdateHintTransIfExistShine(const sead::Vector3f& trans);
    bool isEnableGenerateByCount(s32 count) const;
    void generate(const sead::Vector3f& pos, const sead::Quatf& quat);
    void generate(const sead::Vector3f& pos, const sead::Vector3f& vec);
    bool tryGenerate(const sead::Vector3f& pos, const sead::Quatf& quat, s32 count);
    bool tryGenerate(const sead::Vector3f& pos, const sead::Vector3f& vec, s32 count);
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
    s32 mItemType = -1;
    s32 mGeneratedItemCount = 0;
};
