#pragma once

#include <basis/seadTypes.h>

namespace al {
class CollisionCodeList;
class CollisionCodeRelationList;
}  // namespace al

namespace alCollisionCodeFunction {

al::CollisionCodeList* tyrCreateCollisionCodeList(const char*);
al::CollisionCodeRelationList* tyrGetCollisionRelationList(const al::CollisionCodeList*,
                                                           const char*);
const char* getCodeNameByIdx(const al::CollisionCodeList*, s32 idx);
const char* getDispNameByIdx(const al::CollisionCodeList*, s32 idx);
s32 getIdxByCodeName(const al::CollisionCodeList*, const char*);
bool isExistCodeName(const al::CollisionCodeList*, const char*);
bool isValidUserType(const al::CollisionCodeList*, const char*);
bool judgeIsBetterCodeName(const al::CollisionCodeList*, const char*, const char*, const char*,
                           const char*);
bool judgeIsBetterCodeNameWithAreaName(const al::CollisionCodeRelationList*, const char*,
                                       const char*, const char*, const char*, const char*,
                                       const char*);

}  // namespace alCollisionCodeFunction
