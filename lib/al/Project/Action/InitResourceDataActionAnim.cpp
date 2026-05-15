#include "Project/Action/InitResourceDataActionAnim.h"

#include "Library/Resource/ResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Action/ActionAnimInfo.h"
#include "Project/Anim/AnimInfo.h"
#include "Project/Anim/InitResourceDataAnim.h"

namespace al {
InitResourceDataActionAnim*
InitResourceDataActionAnim::tryCreate(Resource* resource, const InitResourceDataAnim* dataAnim,
                                      const char* suffix) {
    if (!isExistResourceYaml(resource, "ActionAnimCtrl", suffix))
        return nullptr;

    return new InitResourceDataActionAnim(resource, nullptr, suffix);
}

void loadActionAnimDataInfo(ActionAnimDataInfo* data, const ByamlIter& parentIter,
                            ActionAnimCtrlInfo* ctrlInfo, const char* animType,
                            ActionAnimType actionAnimType) {
    ByamlIter iter;
    if (parentIter.tryGetIterByKey(&iter, animType)) {
        iter.tryGetStringByKey(&data->actionName, "Name");
        iter.tryGetBoolByKey(&data->isKeepAnim, "KeepAnim");
        iter.tryGetBoolByKey(&data->isActionAnim, "ActionAnim");
        if (data->isActionAnim)
            ctrlInfo->actionAnimType = actionAnimType;
    }
}

InitResourceDataActionAnim::InitResourceDataActionAnim(Resource* resource,
                                                       const InitResourceDataAnim* dataAnim,
                                                       const char* suffix) {
    ByamlIter actionAnimCtrlIter(findResourceYaml(resource, "ActionAnimCtrl", suffix));

    mAnimInfoCount = actionAnimCtrlIter.getSize();
    mAnimInfos = new ActionAnimCtrlInfo*[mAnimInfoCount];

    for (s32 i = 0; i < mAnimInfoCount; i++) {
        ByamlIter iterIndex;
        actionAnimCtrlIter.tryGetIterByIndex(&iterIndex, i);

        ByamlIter sklIter;
        s32 sklNum = 1;
        if (iterIndex.tryGetIterByKey(&sklIter, "SklAnim") && sklIter.isTypeArray())
            sklNum = sklIter.getSize();

        ActionAnimCtrlInfo* ctrlInfo = new ActionAnimCtrlInfo(sklNum);
        mAnimInfos[i] = ctrlInfo;
        iterIndex.tryGetStringByKey(&ctrlInfo->actionName, "ActionName");

        if (i == 0 && !ctrlInfo->actionName)
            ctrlInfo->actionName = getResourceName(resource);

        if (iterIndex.tryGetIterByKey(&sklIter, "SklAnim")) {
            if (sklIter.isTypeArray()) {
                for (s32 e = 0; e < sklNum; e++) {
                    ActionAnimDataInfo* sklDatas = ctrlInfo->sklDatas;
                    ByamlIter animIter;
                    sklIter.tryGetIterByIndex(&animIter, e);
                    animIter.tryGetStringByKey(&sklDatas[e].actionName, "Name");
                }
            } else {
                ActionAnimDataInfo* sklData = ctrlInfo->sklDatas;
                sklIter.tryGetStringByKey(&sklData->actionName, "Name");
                sklIter.tryGetBoolByKey(&sklData->isKeepAnim, "KeepAnim");
                sklIter.tryGetBoolByKey(&sklData->isActionAnim, "ActionAnim");
                if (sklData->isActionAnim)
                    ctrlInfo->actionAnimType = ActionAnimType::Skl;
            }
        }

        loadActionAnimDataInfo(&ctrlInfo->mtpData, iterIndex, ctrlInfo, "MtpAnim",
                               ActionAnimType::Mtp);

        loadActionAnimDataInfo(&ctrlInfo->mclData, iterIndex, ctrlInfo, "MclAnim",
                               ActionAnimType::Mcl);

        loadActionAnimDataInfo(&ctrlInfo->mtsData, iterIndex, ctrlInfo, "MtsAnim",
                               ActionAnimType::Mts);

        loadActionAnimDataInfo(&ctrlInfo->visData, iterIndex, ctrlInfo, "VisAnim",
                               ActionAnimType::Vis);
    }

    sortCtrlInfo();
}

// BUG: Never sorts the first element in the array
void InitResourceDataActionAnim::sortCtrlInfo() {
    for (s32 i = 1; i < mAnimInfoCount - 1; i++) {
        for (s32 e = i + 1; e < mAnimInfoCount; e++) {
            if (strcmp(mAnimInfos[i]->actionName, mAnimInfos[e]->actionName) > 0) {
                ActionAnimCtrlInfo* tmp = mAnimInfos[i];
                mAnimInfos[i] = mAnimInfos[e];
                mAnimInfos[e] = tmp;
            }
        }
    }
}

}  // namespace al

namespace alAnimFunction {
// TODO: Rename all arguments
bool checkPass(f32 param1, f32 param2, f32 param3, bool param4, f32 param5) {
    if (param5 < 0.0f)
        return false;

    f32 oneMinusThree = param1 - param3;
    if (0.0f < param2 && oneMinusThree < 0.0f && param4) {
        if (param5 > param2)
            return false;

        return oneMinusThree + param2 < param5 || param5 <= param1;
    }
    return oneMinusThree < param5 && param5 <= param1;
}

const al::AnimInfoTable* createAnimInfoTableIfNeed(const al::AnimInfoTable* table1,
                                                   const al::AnimInfoTable* table2) {
    if (table1 == table2 || !table2)
        return table1;

    al::AnimInfoTable* newTable =
        new al::AnimInfoTable(table1->getInfoCount() + table2->getInfoCount());

    for (s32 i = 0; i < table1->getInfoCount(); i++) {
        const al::AnimResInfo& entry = table1->getResInfo(i);
        newTable->add(entry.name, entry.resMaterialAnim, entry.frameMax, entry.isLoop);
    }

    for (s32 i = 0; i < table2->getInfoCount(); i++) {
        const al::AnimResInfo& entry = table2->getResInfo(i);
        newTable->add(entry.name, entry.resMaterialAnim, entry.frameMax, entry.isLoop);
    }

    newTable->sort();
    return newTable;
}

}  // namespace alAnimFunction
