#pragma once

#include <basis/seadTypes.h>

namespace al {
struct CollisionCodeRelationList {
    const char* userType;

    s32 entryNum;
    const char** children;
    const char** parents;
};

static_assert(sizeof(CollisionCodeRelationList) == 0x20);

struct CollisionCodeList {
    const char* category;

    s32 codeNum;
    const char** codeNames;
    const char** displayNames;

    s32 relationListNum;
    const CollisionCodeRelationList* relationLists;
};

static_assert(sizeof(CollisionCodeList) == 0x30);

}  // namespace al

namespace alCollisionCodeFunction {

const al::CollisionCodeList* tyrCreateCollisionCodeList(const char*);
const al::CollisionCodeRelationList* tyrGetCollisionRelationList(const al::CollisionCodeList* list,
                                                                 const char* userType);

const char* getCodeNameByIdx(const al::CollisionCodeList* list, s32 idx);
const char* getDispNameByIdx(const al::CollisionCodeList* list, s32 idx);
s32 getIdxByCodeName(const al::CollisionCodeList* list, const char* name);

bool isExistCodeName(const al::CollisionCodeList* list, const char* name);
bool isValidUserType(const al::CollisionCodeList* list, const char* userType);

bool judgeIsBetterCodeName(const al::CollisionCodeList* list, const char* param_1,
                           const char* param_2, const char* param_3, const char* param_4);
bool judgeIsBetterCodeNameWithAreaName(const al::CollisionCodeRelationList* relationList,
                                       const char* param_1, const char* param_2,
                                       const char* param_3, const char* param_4,
                                       const char* param_5, const char* param_6);

}  // namespace alCollisionCodeFunction
