#include "Project/Action/InitResourceDataActionAnim.h"

#include "Library/Resource/ResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Action/ActionAnimInfo.h"
#include "Project/Anim/AnimInfo.h"
#include "Project/Anim/InitResourceDataAnim.h"

namespace al {
InitResourceDataActionAnim*
InitResourceDataActionAnim::tryCreate(Resource* resource, const InitResourceDataAnim* dataAnim,
                                      const char* resourceName) {
    if (isExistResourceYaml(resource, "ActionAnimCtrl", resourceName))
        return new InitResourceDataActionAnim(resource, nullptr, resourceName);

    return nullptr;
}

void loadActionAnimDataInfo(ActionAnimDataInfo* data, const ByamlIter& parentIter,
                            ActionAnimCtrlInfo* ctrlInfo, const char* animType, s32 partNum) {
    ByamlIter iter;
    if (parentIter.tryGetIterByKey(&iter, animType)) {
        iter.tryGetStringByKey(&data->actionName, "Name");
        iter.tryGetBoolByKey(&data->isKeepAnim, "KeepAnim");
        iter.tryGetBoolByKey(&data->isActionAnim, "ActionAnim");
        if (data->isActionAnim)
            ctrlInfo->partNum = partNum;
    }
}

InitResourceDataActionAnim::InitResourceDataActionAnim(Resource* resource,
                                                       const InitResourceDataAnim* dataAnim,
                                                       const char* resourceYml) {
    ByamlIter actionAnimCtrlIter(findResourceYaml(resource, "ActionAnimCtrl", resourceYml));

    mAnimInfoCount = actionAnimCtrlIter.getSize();
    mAnimInfos = new ActionAnimCtrlInfo*[mAnimInfoCount];

    for (s32 i = 0; i < mAnimInfoCount; i++) {
        ByamlIter iterIndex;
        actionAnimCtrlIter.tryGetIterByIndex(&iterIndex, i);

        ByamlIter sklIter;
        s32 iterKeySize = 1;
        if (iterIndex.tryGetIterByKey(&sklIter, "SklAnim") && sklIter.isTypeArray())
            iterKeySize = sklIter.getSize();

        ActionAnimCtrlInfo* ctrlInfo = new ActionAnimCtrlInfo(iterKeySize);
        mAnimInfos[i] = ctrlInfo;
        iterIndex.tryGetStringByKey(&ctrlInfo->actionName, "ActionName");

        if (i == 0 && !ctrlInfo->actionName)
            ctrlInfo->actionName = getResourceName(resource);

        if (iterIndex.tryGetIterByKey(&sklIter, "SklAnim")) {
            if (sklIter.isTypeArray()) {
                for (s32 e = 0; e < iterKeySize; e++) {
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
                    ctrlInfo->partNum = 0;
            }
        }

        loadActionAnimDataInfo(&ctrlInfo->mtpData, iterIndex, ctrlInfo, "MtpAnim", 2);

        loadActionAnimDataInfo(&ctrlInfo->mclData, iterIndex, ctrlInfo, "MclAnim", 1);

        loadActionAnimDataInfo(&ctrlInfo->mtsData, iterIndex, ctrlInfo, "MtsAnim", 3);

        loadActionAnimDataInfo(&ctrlInfo->visData, iterIndex, ctrlInfo, "VisAnim", 4);
    }

    sortCtrlInfo();
}

// BUG: Never sorts the first element in the array
void InitResourceDataActionAnim::sortCtrlInfo() {
    for (s32 i = 1; i < mAnimInfoCount - 1; i++) {
        for (s32 e = i + 1; e < mAnimInfoCount; e++) {
            if (0 < strcmp(mAnimInfos[i]->actionName, mAnimInfos[e]->actionName)) {
                ActionAnimCtrlInfo* aux = mAnimInfos[i];
                mAnimInfos[i] = mAnimInfos[e];
                mAnimInfos[e] = aux;
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

// TODO: Rename table1 and table2
const al::AnimInfoTable* createAnimInfoTableIfNeed(const al::AnimInfoTable* table1,
                                                   const al::AnimInfoTable* table2) {
    if (table1 == table2 || !table2)
        return table1;

    al::AnimInfoTable* newTable =
        new al::AnimInfoTable(table1->getInfoCount() + table2->getInfoCount());

    for (s32 i = 0; i < table1->getInfoCount(); i++) {
        al::AnimResInfo* entry = table1->getResInfo(i);
        newTable->add(entry->name, entry->resMaterialAnim, entry->frameMax, entry->isLoop);
    }

    for (s32 i = 0; i < table2->getInfoCount(); i++) {
        al::AnimResInfo* entry = table2->getResInfo(i);
        newTable->add(entry->name, entry->resMaterialAnim, entry->frameMax, entry->isLoop);
    }

    newTable->sort();
    return newTable;
}

}  // namespace alAnimFunction
