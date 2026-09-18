#pragma once

#include "Scene/InformationWindowTutorialInfo.h"

namespace al {
class PlayerHolder;
}  // namespace al

class InformationWindowLayout;
class InformationWindowRequestHolder;
class InformationWindowStateHolder;

class InformationWindowRequester {
public:
    InformationWindowRequester(InformationWindowRequestHolder* requestHolder,
                               InformationWindowStateHolder* stateHolder,
                               const al::PlayerHolder* playerHolder);

    virtual void accept(TutorialInfo* info);
    virtual void endStart(const TutorialInfo* info);
    virtual void done(const TutorialInfo* info);
    virtual void cancel(const TutorialInfo* info);
    virtual void retry(TutorialInfo* info);
    virtual void setText(InformationWindowLayout* layout, const TutorialInfo* info);

    void requestAppear(const TutorialInfo& info);
    void requestAppearForce(const TutorialInfo& info);
    void requestCancel(const TutorialInfo& info);
    void requestShowSystem();
    void requestHideSystem();

protected:
    InformationWindowRequestHolder* mRequestHolder = nullptr;
    InformationWindowStateHolder* mStateHolder = nullptr;
    const al::PlayerHolder* mPlayerHolder = nullptr;
};

static_assert(sizeof(InformationWindowRequester) == 0x20);
