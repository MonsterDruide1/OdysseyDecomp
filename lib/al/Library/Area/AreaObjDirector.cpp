#include "Library/Area/AreaObjDirector.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjFactory.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjMtxConnecter.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {

AreaObjDirector::AreaObjDirector() = default;

void AreaObjDirector::init(const AreaObjFactory* factory) {
    mFactory = factory;
    mMtxConnecterHolder = new AreaObjMtxConnecterHolder(0x100);
    s32 nFactoryEntries = mFactory->getNumFactoryEntries();
    mAreaGroups = new AreaObjGroup*[nFactoryEntries];

    for (s32 i = 0; i < nFactoryEntries; i++)
        mAreaGroups[i] = nullptr;
}

void AreaObjDirector::endInit() {}

void AreaObjDirector::update() {
    if (mMtxConnecterHolder)
        mMtxConnecterHolder->update();
}

void AreaObjDirector::placement(const AreaInitInfo& initInfo) {
    createAreaObjGroup(initInfo);
    createAreaObjGroupBuffer();
    placementAreaObj(initInfo);
}

void AreaObjDirector::placement(const AreaInitInfo* initInfoArray, s32 initInfoCount) {
    for (s32 i = 0; i < initInfoCount; i++)
        createAreaObjGroup(initInfoArray[i]);

    createAreaObjGroupBuffer();

    for (s32 i = 0; i < initInfoCount; i++)
        placementAreaObj(initInfoArray[i]);
}

void AreaObjDirector::createAreaObjGroup(const AreaInitInfo& initInfo) {
    PlacementInfo placementInfo(initInfo);

    s32 pInfoCount = getCountPlacementInfo(placementInfo);
    for (s32 i = 0; i < pInfoCount; i++) {
        PlacementInfo placementInfo2;
        tryGetPlacementInfoByIndex(&placementInfo2, placementInfo, i);
        const char* pInfoName = nullptr;
        tryGetObjectName(&pInfoName, placementInfo2);

        AreaCreatorFunction creatorFunc = nullptr;
        s32 entryIndex = mFactory->getEntryIndex(&creatorFunc, pInfoName);

        if (!creatorFunc)
            continue;
        if (!mAreaGroups[entryIndex]) {
            s32 size = mFactory->tryFindAddBufferSize(pInfoName);
            mAreaGroups[entryIndex] = new AreaObjGroup(pInfoName, size);
        }

        mAreaGroups[entryIndex]->incrementCount();
    }
}

void AreaObjDirector::createAreaObjGroupBuffer() {
    for (s32 i = 0; i < mFactory->getAreaGroupCount(); i++) {
        const AreaGroupInfo& addBuffer = mFactory->getAreaGroupInfo()[i];
        AreaCreatorFunction creatorFunc = nullptr;
        s32 entryIndex = mFactory->getEntryIndex(&creatorFunc, addBuffer.name);
        if (!mAreaGroups[entryIndex])
            mAreaGroups[entryIndex] = new AreaObjGroup(addBuffer.name, addBuffer.size);
    }

    s32 areaGroupCount = 0;
    s32 nEntries = mFactory->getNumFactoryEntries();
    for (s32 i = 0; i < nEntries; i++) {
        if (mAreaGroups[i] == nullptr)
            continue;
        mAreaGroups[i]->createBuffer();
        areaGroupCount++;

        for (s32 areaGroupIndex = i; areaGroupIndex > 0; areaGroupIndex--) {
            AreaObjGroup* prevAreaObjGroup = mAreaGroups[areaGroupIndex - 1];
            if (prevAreaObjGroup) {
                if (strcmp(mAreaGroups[areaGroupIndex]->getName(), prevAreaObjGroup->getName()) >
                    -1)
                    break;
            }
            mAreaGroups[areaGroupIndex - 1] = mAreaGroups[areaGroupIndex];
            mAreaGroups[areaGroupIndex] = prevAreaObjGroup;
        }
    }
    mAreaGroupCount = areaGroupCount;
}

void AreaObjDirector::placementAreaObj(const AreaInitInfo& initInfo) {
    PlacementInfo pInfo(initInfo);
    s32 pInfoCount = getCountPlacementInfo(pInfo);
    for (s32 j = 0; j < pInfoCount; j++) {
        PlacementInfo pInfo2;
        tryGetPlacementInfoByIndex(&pInfo2, pInfo, j);
        const char* pInfoName = nullptr;
        tryGetObjectName(&pInfoName, pInfo2);

        AreaCreatorFunction creatorFunc = nullptr;
        mFactory->getEntryIndex(&creatorFunc, pInfoName);
        if (creatorFunc == nullptr)
            continue;

        const char* displayName;
        getDisplayName(&displayName, pInfo2);
        AreaObj* areaObj = creatorFunc(displayName);
        AreaInitInfo initInfo2(pInfo2, initInfo);
        areaObj->init(initInfo2);

        AreaObjGroup* areaGroup = getAreaObjGroup(pInfoName);

        areaGroup->registerAreaObj(areaObj);
        mMtxConnecterHolder->tryAddArea(areaObj, pInfo2);
    }
}

AreaObjGroup* AreaObjDirector::getAreaObjGroup(const char* name) const {
    s32 lower = 0;
    s32 upper = mAreaGroupCount;
    while (lower < upper) {
        s32 i = (lower + upper) / 2;
        s32 equalString = strcmp(name, mAreaGroups[i]->getName());
        if (equalString == 0)
            return mAreaGroups[i];
        else if (equalString > 0)
            lower = i + 1;
        else
            upper = i;
    }
    return nullptr;
}

bool AreaObjDirector::isExistAreaGroup(const char* name) const {
    return getAreaObjGroup(name) != nullptr;
}

AreaObj* AreaObjDirector::getInVolumeAreaObj(const char* name, const sead::Vector3f& position) {
    AreaObjGroup* areaGroup = getAreaObjGroup(name);
    if (!areaGroup)
        return nullptr;
    return areaGroup->getInVolumeAreaObj(position);
}

AreaObjMtxConnecterHolder* AreaObjDirector::getMtxConnecterHolder() const {
    return mMtxConnecterHolder;
}

}  // namespace al
