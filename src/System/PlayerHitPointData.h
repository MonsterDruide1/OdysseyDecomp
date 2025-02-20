#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class PlayerHitPointData : public ByamlSave {
public:
    PlayerHitPointData();
    void setKidsModeFlag(bool kidsMode);
    void init();
    void recoverMax();
    s32 getCurrent() const;
    s32 getMaxCurrent() const;
    s32 getMaxWithItem() const;
    s32 getMaxWithoutItem() const;
    bool isMaxCurrent() const;
    bool isMaxWithItem() const;
    void getMaxUpItem();
    void recover();
    void recoverForDebug();
    void damage();
    void kill();
    void forceNormalMode();
    void endForceNormalMode();
    bool isForceNormalMode() const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& reader) override;

private:
    bool mIsKidsMode = false;
    s32 mCurrentHealth = 0;
    bool mIsHaveMaxUpItem = false;
    bool mIsForceNormalMode = false;
};
