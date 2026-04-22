#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
}  // namespace al

class CapMessageGameDataChecker;
class CapMessageKeeper;
class CapMessageLayout;
class CapMessagePlacement;
class CapMessageShowInfo;

class CapMessageDirector : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CapMessageDirector;

    CapMessageDirector();
    ~CapMessageDirector() override;

    const char* getSceneObjName() const override;
    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;

    void exeWait();
    bool tryStartShowPlacement();
    void exeDelayPlacement();
    CapMessagePlacement* findPlacement() const;
    void exeShowPlacement();
    void forceEndInner();
    void exeShowSystemLow();
    void exeShowSystem();
    void exeShowSystemContinue();
    void exeEnd();
    bool isShow(const char* labelName) const;
    bool isDelay(const char* labelName) const;
    bool isActive(const char* labelName) const;
    s32 registerCapMessagePlacement(CapMessagePlacement* placement);
    bool tryShowMessageSystem(const CapMessageShowInfo* showInfo,
                              const CapMessageGameDataChecker* checker);
    bool tryCheck(const CapMessageGameDataChecker* checker) const;
    bool tryShowMessageSystemLow(const CapMessageShowInfo* showInfo,
                                 const CapMessageGameDataChecker* checker);
    bool tryShowMessageSystemContinue(const CapMessageShowInfo* showInfo,
                                      const CapMessageGameDataChecker* checker);
    void endCapMessageSystemContinue();
    void invalidateAppearCapMessage();
    void validateAppearCapMessage();
    void forceEnd();

private:
    CapMessageLayout* mLayout;
    al::DeriveActorGroup<CapMessagePlacement>* mPlacementGroup;
    CapMessagePlacement* mCurrentPlacement;
    CapMessageKeeper* mMessageKeeper;
    bool mIsAppearCapMessageValid;
};

static_assert(sizeof(CapMessageDirector) == 0x138, "CapMessageDirector");

namespace CapMessageDirectorFunction {
s32 registerCapMessagePlacement(CapMessagePlacement* placement);
bool tryShowCapMessageSystem(const al::IUseSceneObjHolder* sceneObjHolder,
                             const CapMessageShowInfo* showInfo,
                             const CapMessageGameDataChecker* checker);
bool tryShowCapMessageSystemLow(const al::IUseSceneObjHolder* sceneObjHolder,
                                const CapMessageShowInfo* showInfo,
                                const CapMessageGameDataChecker* checker);
bool tryShowCapMessageSystemContinue(const al::IUseSceneObjHolder* sceneObjHolder,
                                     const CapMessageShowInfo* showInfo,
                                     const CapMessageGameDataChecker* checker);
void endCapMessageSystemContinue(const al::IUseSceneObjHolder* sceneObjHolder);
void invalidateAppearCapMessageNew(const al::IUseSceneObjHolder* sceneObjHolder);
void validateAppearCapMessageNew(const al::IUseSceneObjHolder* sceneObjHolder);
void forceEndCapMessage(const al::IUseSceneObjHolder* sceneObjHolder);
bool tryCancelCapMessageNew(const al::IUseSceneObjHolder* sceneObjHolder, const char* labelName);
bool isShowCapMessage(const al::IUseSceneObjHolder* sceneObjHolder, const char* labelName);
bool isDelayCapMessage(const al::IUseSceneObjHolder* sceneObjHolder, const char* labelName);
bool isActiveCapMessage(const al::IUseSceneObjHolder* sceneObjHolder, const char* labelName);
CapMessageKeeper* getCapMessageKeeper(const al::IUseSceneObjHolder* sceneObjHolder);
}  // namespace CapMessageDirectorFunction
