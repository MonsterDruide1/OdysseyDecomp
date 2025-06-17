#include "Library/Resource/ActorResource.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitResourceData.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"
#include "Project/Action/InitResourceDataAction.h"
#include "Project/Anim/InitResourceDataAnim.h"

namespace al {

ActorResource::ActorResource(const sead::SafeString& name, Resource* modelResource,
                             Resource* animResource)
    : mModelRes(modelResource), mAnimRes(animResource) {
    mName = name;
}

void ActorResource::initResourceData(const char* suffix, bool mergeAnim) {
    StringTmp<64> fileSuffixName = "";
    if (!tryGetActorInitFileSuffixName(&fileSuffixName, mModelRes, "ActionAnimCtrl", suffix)) {
        const char* newSuffix = "";
        if (suffix)
            newSuffix = suffix;
        fileSuffixName.copy(newSuffix);
    }

    StringTmp<128> fileName = "";
    createFileNameBySuffix(&fileName, "ActionAnimCtrl", fileSuffixName.cstr());

    Resource* mainRes = mModelRes;
    Resource* animRes = mAnimRes;
    if (animRes) {
        mHasAnimData = true;
        if (mergeAnim) {
            if (mainRes->getResData() && mainRes->getResData()->getAnimData()) {
                if (animRes->getResData() && animRes->getResData()->getAnimData()) {
                    mAnimResData = InitResourceDataAnim::tryCreate(mainRes, animRes);
                    mActionResData = InitResourceDataAction::tryCreate(animRes, mAnimResData,
                                                                       fileSuffixName.cstr());
                    return;
                }
            }
        }

        mainRes = animRes;
    }

    ActorInitResourceData* resData = mainRes->getResData();
    if (!resData)
        return;
    mAnimResData = resData->getAnimData();

    if (suffix && !isEqualString(fileName.cstr(), "ActionAnimCtrl") &&
        isExistResourceYaml(mainRes, fileName.cstr(), nullptr)) {
        mActionResData =
            InitResourceDataAction::tryCreate(mainRes, mAnimResData, fileSuffixName.cstr());
    } else {
        mActionResData = resData->getDataAction();
    }
}

}
