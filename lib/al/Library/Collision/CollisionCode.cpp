#include "Library/Collision/CollisionCode.h"

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"

namespace alCollisionCodeFunction {

const al::CollisionCodeList* tyrCreateCollisionCodeList(const char* category) {
    al::Resource* resource = al::findOrCreateResource("SystemData/CollisionAttributeList", nullptr);

    al::CollisionCodeList* codeList = new al::CollisionCodeList;

    {
        al::ByamlIter codeIter(al::findResourceYaml(resource, "CollisionAttributeList", nullptr));
        al::ByamlIter codeListIter;
        if (!codeIter.tryGetIterByKey(&codeListIter, category))
            // BUG: This leaks the allocated list
            return nullptr;

        s32 codeNum = codeListIter.getSize();

        const char** codeNames = new const char*[codeNum];
        const char** dispNames = new const char*[codeNum];

        for (s32 listIdx = 0; listIdx < codeNum; listIdx++) {
            al::ByamlIter entryIter;
            codeListIter.tryGetIterByIndex(&entryIter, listIdx);

            entryIter.tryGetStringByKey(&codeNames[listIdx], "name_en");

            al::StringTmp<512> dispName;
            dispName.format("%s(%s)", codeNames[listIdx], nullptr);

            size_t strLen = (size_t)dispName.calcLength() + 8u;
            char* copy = new char[strLen];
            al::copyString(copy, dispName.cstr(), strLen);
            dispNames[listIdx] = copy;
        }

        codeList->codeNum = codeNum;
        codeList->codeNames = codeNames;
        codeList->displayNames = dispNames;
        codeList->category = category;
    }

    al::ByamlIter relationListsIter(
        al::findResourceYaml(resource, "CollisionAttributeList_Relation", nullptr));
    s32 relationNum = relationListsIter.getSize();

    al::CollisionCodeRelationList* relationLists = new al::CollisionCodeRelationList[relationNum];

    for (s32 relationIdx = 0; relationIdx < relationNum; relationIdx++) {
        al::ByamlIter listIter;
        relationListsIter.tryGetIterByIndex(&listIter, relationIdx);

        auto* self = &relationLists[relationIdx];

        listIter.tryGetStringByKey(&self->userType, "UserType");
        listIter.tryGetIterByKey(&listIter, "RelationList");

        s32 num = listIter.getSize();
        self->entryNum = num;
        self->children = new const char*[num];
        self->parents = new const char*[num];

        for (s32 listIdx = 0; listIdx < num; listIdx++) {
            al::ByamlIter entryIter;
            listIter.tryGetIterByIndex(&entryIter, listIdx);

            entryIter.tryGetStringByKey(&self->children[listIdx], "child");
            entryIter.tryGetStringByKey(&self->parents[listIdx], "parent");
        }
    }

    codeList->relationListNum = relationNum;
    codeList->relationLists = relationLists;

    return codeList;
}

const al::CollisionCodeRelationList* tyrGetCollisionRelationList(const al::CollisionCodeList* list,
                                                                 const char* userType) {
    for (s32 idx = 0; idx < list->relationListNum; idx++)
        if (al::isEqualString(list->relationLists[idx].userType, userType))
            return &list->relationLists[idx];

    return nullptr;
}

const char* getCodeNameByIdx(const al::CollisionCodeList* list, s32 idx) {
    return list->codeNames[idx];
}

const char* getDispNameByIdx(const al::CollisionCodeList* list, s32 idx) {
    return list->displayNames[idx];
}

s32 getIdxByCodeName(const al::CollisionCodeList* list, const char* name) {
    for (s32 idx = 0; idx < list->codeNum; idx++)
        if (al::isEqualString(name, list->codeNames[idx]))
            return idx;

    return -1;
}

bool isExistCodeName(const al::CollisionCodeList* list, const char* name) {
    return getIdxByCodeName(list, name) != -1;
}

bool isValidUserType(const al::CollisionCodeList* list, const char* userType) {
    for (s32 idx = 0; idx < list->relationListNum; idx++)
        if (al::isEqualString(list->relationLists[idx].userType, userType))
            return true;

    return false;
}

// TODO: Figure out parameter names
bool judgeIsBetterCodeName(const al::CollisionCodeList* list, const char* param_1,
                           const char* param_2, const char* param_3, const char* param_4) {
    if (al::isEqualString(param_1, param_2))
        return false;
    if (al::isEqualString(param_1, param_3))
        return true;

    s32 relationIdx = -1;
    for (s32 idx = 0; idx < list->relationListNum; idx++) {
        if (al::isEqualString(list->relationLists[idx].userType, param_4)) {
            relationIdx = idx;
            break;
        }
    }

    s32 entryNum = list->relationLists[relationIdx].entryNum;
    for (s32 entryIdx = 0; entryIdx < entryNum; entryIdx++) {
        if (al::isEqualString(list->relationLists[relationIdx].children[entryIdx], param_1)) {
            auto* parent = list->relationLists[relationIdx].parents[entryIdx];
            if (parent && !al::isEqualString(parent, param_2) && al::isEqualString(parent, param_3))
                return true;

            return false;
        }
    }

    return false;
}

static s32 FUN_7100846834(const char* param_1, const char* param_2, const char* param_3,
                          const char* param_4, const char* param_5);

// TODO: Figure out parameter names
bool judgeIsBetterCodeNameWithAreaName(const al::CollisionCodeRelationList* relationList,
                                       const char* param_1, const char* param_2,
                                       const char* param_3, const char* param_4,
                                       const char* param_5, const char* param_6) {
    s32 num = relationList->entryNum;
    const char* parent = nullptr;
    for (s32 idx = 0; idx < num; idx++) {
        if (al::isEqualString(relationList->children[idx], param_1)) {
            parent = relationList->parents[idx];
            break;
        }
    }

    return FUN_7100846834(param_1, param_2, parent, param_3, param_4) <
           FUN_7100846834(param_1, param_2, parent, param_5, param_6);
}

// TODO: Figure out parameter, locals and function name
static s32 FUN_7100846834(const char* param_1, const char* param_2, const char* parent,
                          const char* param_4, const char* param_5) {
    bool bVar2 = true;
    if (param_2 && param_5)
        bVar2 = !al::isEqualString(param_2, param_5);

    bool bVar3 = al::isEqualString(param_1, param_4);

    bool bVar4 = false;
    if (param_4)
        bVar4 = !al::isEqualString(param_4, "");

    bool bVar5 = false;
    if (parent)
        bVar5 = al::isEqualString(parent, param_4);

    bool bVar1 = !(bVar2 || bVar4);

    if (!bVar2 && bVar3)
        return 6;
    if (!bVar2 && bVar5)
        return 5;

    // Not pretty, but doesn't like to match otherwise
    s32 status = 3;
    if (!bVar2 && !bVar4) {
        status = 4;
    } else if (!bVar3 && !bVar1) {
        if (!bVar5) {
            if (!al::isEqualString(param_4, "") || !al::isEqualString(param_5, ""))
                return 0;

            status = 1;

        } else {
            status = 2;
        }
    }
    return status;
}

}  // namespace alCollisionCodeFunction
