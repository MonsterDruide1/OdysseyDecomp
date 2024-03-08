#include "al/include/Library/Area/AreaObjDirector.h"

#include "al/include/Library/Area/AreaInitInfo.h"
#include "al/include/Library/Area/AreaObj.h"
#include "al/include/Library/Area/AreaObjFactory.h"
#include "al/include/Library/Area/AreaObjGroup.h"
#include "al/include/Library/Area/AreaObjMtxConnecter.h"
#include "al/include/Library/Placement/PlacementFunction.h"
#include "al/include/Library/Placement/PlacementInfo.h"

namespace al {

AreaObjDirector::AreaObjDirector() {}

void AreaObjDirector::init(const al::AreaObjFactory* factory) {
    mFactory = factory;
    mMtxConnecterHolder = new AreaObjMtxConnecterHolder(0x100);
    s32 nFactoryEntries = mFactory->getNumFactoryEntries();
    s32 areaGroupsSize = nFactoryEntries;
    mAreaGroups = new AreaObjGroup*[areaGroupsSize];

    for (s32 i = 0; i < nFactoryEntries; i++) {
        mAreaGroups[i] = nullptr;
    }
}

void AreaObjDirector::endInit() {}

void AreaObjDirector::update() {
    if (mMtxConnecterHolder == nullptr)
        return;
    mMtxConnecterHolder->update();
}

void AreaObjDirector::placement(const al::AreaInitInfo& initInfo) {
    createAreaObjGroup(initInfo);
    createAreaObjGroupBuffer();
    placementAreaObj(initInfo);
}

void AreaObjDirector::placement(const al::AreaInitInfo* initInfoArray, s32 initInfoCount) {
    for (s32 i = 0; i < initInfoCount; i++) {
        createAreaObjGroup(initInfoArray[i]);
    }

    createAreaObjGroupBuffer();

    for (s32 i = 0; i < initInfoCount; i++) {
        placementAreaObj(initInfoArray[i]);
    }
}

void AreaObjDirector::createAreaObjGroup(const al::AreaInitInfo& initInfo) {
    al::PlacementInfo placementInfo(initInfo);

    s32 pInfoCount = al::getCountPlacementInfo(placementInfo);
    for (s32 i = 0; i < pInfoCount; i++) {
        al::PlacementInfo placementInfo2;
        al::tryGetPlacementInfoByIndex(&placementInfo2, placementInfo, i);
        const char* pInfoName = nullptr;
        al::tryGetObjectName(&pInfoName, placementInfo2);

        AreaObjCreatorFunction creatorFunc = nullptr;
        s32 factoryIx = mFactory->getEntryIndex(pInfoName, &creatorFunc);

        if (creatorFunc == nullptr)
            continue;
        if (mAreaGroups[factoryIx] == nullptr) {
            s32 size = mFactory->tryFindAddBufferSize(pInfoName);
            mAreaGroups[factoryIx] = new AreaObjGroup(pInfoName, size);
        }

        mAreaGroups[factoryIx]->incrementCount();
    }
}

void AreaObjDirector::createAreaObjGroupBuffer() {
    for (s32 i = 0; i < mFactory->getAreaGroupCount(); i++) {
        AreaGroupInfo* addBuffer = mFactory->getAreaGroupInfo() + i;
        AreaObjCreatorFunction creatorFunc = nullptr;
        s32 entryIndex = mFactory->getEntryIndex(addBuffer->name, &creatorFunc);
        if (mAreaGroups[entryIndex] == nullptr)
            mAreaGroups[entryIndex] = new AreaObjGroup(addBuffer->name, addBuffer->size);
    }

    s32 areaGroupCount = 0;
    s32 nEntries = mFactory->getNumFactoryEntries();
    for (s32 i = 0; i < nEntries; i++) {
        if (mAreaGroups[i] == nullptr)
            continue;
        mAreaGroups[i]->createBuffer();
        areaGroupCount++;

        for (s32 areaGroupIx = i; areaGroupIx > 0; areaGroupIx--) {
            AreaObjGroup* prevAreaObjGroup = mAreaGroups[areaGroupIx - 1];
            if (prevAreaObjGroup != nullptr) {
                if (strcmp(mAreaGroups[areaGroupIx]->getName(), prevAreaObjGroup->getName()) > -1)
                    break;
            }
            mAreaGroups[areaGroupIx - 1] = mAreaGroups[areaGroupIx];
            mAreaGroups[areaGroupIx] = prevAreaObjGroup;
        }
    }
    mAreaGroupCount = areaGroupCount;
}

void AreaObjDirector::placementAreaObj(const al::AreaInitInfo& initInfo) {
    PlacementInfo pInfo(initInfo);
    s32 pInfoCount = getCountPlacementInfo(pInfo);
    for (s32 j = 0; j < pInfoCount; j++) {
        PlacementInfo pInfo2;
        tryGetPlacementInfoByIndex(&pInfo2, pInfo, j);
        const char* pInfoName = nullptr;
        tryGetObjectName(&pInfoName, pInfo2);

        AreaObjCreatorFunction creatorFunc = nullptr;
        mFactory->getEntryIndex(pInfoName, &creatorFunc);
        if (creatorFunc == nullptr)
            continue;

        const char* displayName;
        getDisplayName(&displayName, pInfo2);
        AreaObj* areaObj = creatorFunc(displayName);
        AreaInitInfo initInfo2(pInfo2, initInfo);
        areaObj->init(initInfo2);

        AreaObjGroup* areaGroup = _getAreaObjGroup(pInfoName);

        areaGroup->registerAreaObj(areaObj);
        mMtxConnecterHolder->tryAddArea(areaObj, pInfo2);
    }
}

AreaObjGroup* AreaObjDirector::getAreaObjGroup(const char* name) const {
    // return _getAreaObjGroup(name); // doesn't match
    s32 lower = 0;
    s32 upper = mAreaGroupCount;
    while (lower < upper) {
        s32 i = (lower + upper) / 2;
        s32 equalString = strcmp(name, mAreaGroups[i]->getName());
        if (equalString == 0) {
            return mAreaGroups[i];
        } else if (equalString > 0) {
            lower = i + 1;
        } else
            upper = i;
    }
    return nullptr;
}

bool AreaObjDirector::isExistAreaGroup(const char* name) const {
    return _getAreaObjGroup(name) != nullptr;
}

AreaObj* AreaObjDirector::getInVolumeAreaObj(const char* name, const sead::Vector3f& position) {
    AreaObjGroup* areaGroup = _getAreaObjGroup(name);
    if (areaGroup == nullptr)
        return nullptr;
    return areaGroup->getInVolumeAreaObj(position);
}

AreaObjMtxConnecterHolder* AreaObjDirector::getMtxConnecterHolder() const {
    return mMtxConnecterHolder;
}

}  // namespace al
