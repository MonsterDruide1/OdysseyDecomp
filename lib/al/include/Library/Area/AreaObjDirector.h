#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include "al/include/Library/Area/AreaObjGroup.h"

namespace al {
class AreaObjFactory;
class AreaObjMtxConnecterHolder;
class AreaObjGroup;
class AreaObj;
class AreaInitInfo;

class AreaObjDirector {
public:
    AreaObjDirector();
    void init(const AreaObjFactory* factory);
    void endInit();
    void update();
    void placement(const AreaInitInfo& initInfo);
    void placement(const AreaInitInfo* initInfo, s32 initInfoCount);
    void createAreaObjGroup(const AreaInitInfo& initInfo);
    void createAreaObjGroupBuffer();
    void placementAreaObj(const AreaInitInfo& initInfo);
    AreaObjGroup* getAreaObjGroup(const char* name) const;
    bool isExistAreaGroup(const char* name) const;
    AreaObj* getInVolumeAreaObj(const char* name, const sead::Vector3f& position);
    AreaObjMtxConnecterHolder* getMtxConnecterHolder() const;

private:
    const AreaObjFactory* mFactory = nullptr;
    AreaObjMtxConnecterHolder* mMtxConnecterHolder = nullptr;
    AreaObjGroup** mAreaGroups = nullptr;
    u32 mAreaGroupCount = 0;

    AreaObjGroup* _getAreaObjGroup(const char* name) const {
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
};
}  // namespace al
