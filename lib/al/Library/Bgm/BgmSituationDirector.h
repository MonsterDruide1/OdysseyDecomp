#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
class BgmDataBase;
class BgmDirector;

class BgmSituationDirector {
public:
    BgmSituationDirector(BgmDataBase* dataBase);

    void update();
    void forceEndSituation(BgmDirector* director);
    void tryAddHoldUpSituation(const char* situationName);
    void tryAddForceEndSituation(const char* situationName);
    void tryRemoveHoldUpSituation(const char* situationName);
    void tryRemoveForceEndSituation(const char* situationName);
    void clear();
    void startHoldUpSituation(BgmDirector* director);
    void startSituation(const char* situationName, bool isApply, bool isForce, bool isForceStart);
    void endSituation(const char* situationName, bool isApply, bool isForce);
    void disableSituationChange();
    void enableSituationChange(bool isForce);
    const char* getActiveSituationName(s32 index) const;
    const char* getDeactiveSituationName(s32 index) const;
    void takeBgmSituationSnapShot();
    void applyBgmSituationSnapShot();
    void applyNoAppliedSituationRecord();

    s32 getActiveSituationNum() const { return mActiveSituations->size(); }

    s32 getDeactiveSituationNum() const { return mDeactiveSituations->size(); }

    bool isDisableSituationChange() const { return mIsDisableSituationChange; }

private:
    u8 _0[0x18] = {};
    sead::PtrArray<const char*>* mActiveSituations = nullptr;
    sead::PtrArray<const char*>* mDeactiveSituations = nullptr;
    u8 _28[0x28] = {};
    bool mIsDisableSituationChange = false;
    u8 _51[7] = {};
    u8 _58[0x38] = {};
};

static_assert(sizeof(BgmSituationDirector) == 0x90);

}  // namespace al
