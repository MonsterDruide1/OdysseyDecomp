#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
class AreaObjGroup;
class IUseAreaObj;
class AreaObjFilterBase;
class AreaObjFindCallBack;

class AreaObjFilterBase {
public:
    virtual bool isValid(AreaObj* areaObj) = 0;
};

class AreaObjFindCallBack {
public:
    virtual void findArea(const AreaObj* areaObj) = 0;
};

AreaObj* tryFindAreaObj(const IUseAreaObj* areaUser, const char* name,
                        const sead::Vector3f& position);
AreaObj* tryFindAreaObjWithFilter(const IUseAreaObj* areaUser, const char* name,
                                  const sead::Vector3f& position, AreaObjFilterBase* filter);
bool tryFindAreaObjAll(const IUseAreaObj* areaUser, const char* name,
                       const sead::Vector3f& position, AreaObjFindCallBack* callBack);
AreaObjGroup* tryFindAreaObjGroup(const IUseAreaObj* areaUser, const char* name);
bool isInAreaObj(const AreaObjGroup* group, const sead::Vector3f& position);
AreaObj* tryGetAreaObj(const AreaObjGroup* group, const sead::Vector3f& position);
bool isInAreaPos(const AreaObj* areaObj, const sead::Vector3f& position);
bool isInAreaObj(const IUseAreaObj* areaUser, const char* name, const sead::Vector3f& position);

bool isInDeathArea(const IUseAreaObj*, const sead::Vector3f&);

bool tryGetAreaObjArg(s32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(f32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(bool*, const AreaObj*, const char*);
bool tryGetAreaObjStringArg(const char**, const AreaObj*, const char*);

const sead::Matrix34f& getAreaObjBaseMtx(const AreaObj*);
}  // namespace al
