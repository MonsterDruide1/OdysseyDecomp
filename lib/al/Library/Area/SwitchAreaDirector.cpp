#include "Library/Area/SwitchAreaDirector.h"

#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/SwitchKeepOnAreaGroup.h"
#include "Library/Area/SwitchOnAreaGroup.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraUtil.h"

namespace al {
// TODO
class SwitchAreaTargetInfo {
public:
    SwitchAreaTargetInfo(s32, s32);
    SwitchAreaTargetInfo(sead::Vector3f*, s32);

    void update(const PlayerHolder*, const SceneCameraInfo*);

public:
    sead::Vector3f* _0;
    s32 _8;
    unsigned char padding[0x24];
};

static_assert(sizeof(SwitchAreaTargetInfo) == 0x30);

SwitchAreaDirector* SwitchAreaDirector::tryCreate(AreaObjDirector* areaObjDirector,
                                                  const PlayerHolder* playerHolder,
                                                  const CameraDirector* cameraDirector,
                                                  s32 switchOnAreaGroupCount,
                                                  s32 switchKeepOnAreaGroupCount) {
    return new SwitchAreaDirector(areaObjDirector, playerHolder, cameraDirector,
                                  switchOnAreaGroupCount, switchKeepOnAreaGroupCount);
}

// NON_MATCHING
void SwitchAreaDirector::update() {
    if (!mHasSwitchAreas)
        return;

    mSwitchAreaTargetInfo->update(mPlayerHolder, mCameraDirector->getSceneCameraInfo());

    if (mSwitchOnAreaGroups.size() != 0) {
        s64 lVar4 = mSwitchOnAreaGroups.size() * 8L;
        auto** ppSVar2 = mSwitchOnAreaGroups.data();
        do {
            (*ppSVar2)->update(mSwitchAreaTargetInfo->_0, mSwitchAreaTargetInfo->_8);
            lVar4 -= 8L;
            ppSVar2++;
        } while (lVar4 != 0L);
    }
    //    for (s32 i = mSwitchOnAreaGroups.size(); i != 0; i--)
    //        mSwitchOnAreaGroups(i)->update(mSwitchAreaTargetInfo->_0, mSwitchAreaTargetInfo->_8);

    getCameraAt(mCameraDirector->getSceneCameraInfo(), 0);

    if (mSwitchKeepOnAreaGroups.size() != 0) {
        s64 lVar4 = mSwitchKeepOnAreaGroups.size() * 8L;
        auto** ppSVar2 = mSwitchKeepOnAreaGroups.data();
        do {
            (*ppSVar2)->update(mSwitchAreaTargetInfo);
            lVar4 -= 8L;
            ppSVar2++;
        } while (lVar4 != 0L);
    }

    //    for (s32 i = mSwitchKeepOnAreaGroups.size(); i != 0; i--)
    //        mSwitchKeepOnAreaGroups(i)->update(mSwitchAreaTargetInfo);
}

void SwitchAreaDirector::endInit() {
    mHasSwitchAreas = mSwitchOnAreaGroups.size() != 0 || mSwitchKeepOnAreaGroups.size() != 0;
}

void SwitchAreaDirector::registerSwitchOnAreaGroup(SwitchOnAreaGroup* switchOnAreaGroup) {
    mSwitchOnAreaGroups.pushBack(switchOnAreaGroup);
}

void SwitchAreaDirector::registerSwitchKeepOnAreaGroup(
    SwitchKeepOnAreaGroup* switchKeepOnAreaGroup) {
    mSwitchKeepOnAreaGroups.pushBack(switchKeepOnAreaGroup);
}

SwitchAreaDirector::SwitchAreaDirector(AreaObjDirector* areaObjDirector,
                                       const PlayerHolder* playerHolder,
                                       const CameraDirector* cameraDirector,
                                       s32 switchOnAreaGroupCount, s32 switchKeepOnAreaGroupCount)
    : mAreaObjDirector(areaObjDirector), mPlayerHolder(playerHolder),
      mCameraDirector(cameraDirector) {
    mSwitchAreaTargetInfo = new SwitchAreaTargetInfo(16, 4);

    if (switchOnAreaGroupCount > 0) {
        mSwitchOnAreaGroups.allocBuffer(switchOnAreaGroupCount, nullptr);
        AreaObjGroup* areaObjGroup = areaObjDirector->getAreaObjGroup("SwitchOnArea");
        if (areaObjGroup)
            registerSwitchOnAreaGroup(new SwitchOnAreaGroup(areaObjGroup));
    }

    if (switchKeepOnAreaGroupCount > 0) {
        mSwitchKeepOnAreaGroups.allocBuffer(switchKeepOnAreaGroupCount, nullptr);
        AreaObjGroup* areaObjGroup = areaObjDirector->getAreaObjGroup("SwitchKeepOnArea");
        if (areaObjGroup)
            registerSwitchKeepOnAreaGroup(new SwitchKeepOnAreaGroup(areaObjGroup));
    }
}

AreaObjDirector* SwitchAreaDirector::getAreaObjDirector() const {
    return mAreaObjDirector;
}
}  // namespace al
