#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

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
    void placement(const AreaInitInfo& initInfo, s32);
    void createAreaObjGroup(const AreaInitInfo& initInfo);
    void createAreaObjGroupBuffer(const AreaInitInfo& initInfo);
    void placementAreaObj(const AreaInitInfo& initInfo);
    AreaObjGroup* getAreaObjGroup(const char* name);
    bool isExistAreaGroup(const char* name);
    AreaObj* getInVolumeAreaObj(const char* name, const sead::Vector3f& position);
    AreaObjMtxConnecterHolder* getMtxConnecterHolder();

private:
    AreaObjFactory* mFactory;
    AreaObjMtxConnecterHolder* mMtxConnecterHolder;
    AreaObjGroup** mAreaGroups;
    u32 mAreaGroupCount;
};
}  // namespace al
