#pragma once

#include "Library/Nerve/IUseNerve.h"

#include "Scene/InformationWindowRequester.h"

namespace al {
class IUseSceneObjHolder;
class NerveKeeper;
class PlayerHolder;
}  // namespace al

class InformationWindowObjectRequester : public InformationWindowRequester, public al::IUseNerve {
public:
    InformationWindowObjectRequester(InformationWindowRequestHolder* requestHolder,
                                     InformationWindowStateHolder* stateHolder,
                                     const al::PlayerHolder* playerHolder);

    void update();
    void requestAppearObject(const al::IUseSceneObjHolder* sceneObjHolder, const char* name);
    void requestCloseObject(const al::IUseSceneObjHolder* sceneObjHolder, const char* name);
    void exeShow();
    void exeHide();

    al::NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

private:
    al::NerveKeeper* mNerveKeeper = nullptr;
};

static_assert(sizeof(InformationWindowObjectRequester) == 0x30);
