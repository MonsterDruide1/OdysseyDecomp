#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseNerve;
class Nerve;
class NerveStateCtrl;
class NerveActionCtrl;

class NerveKeeper {
public:
    NerveKeeper(IUseNerve* parent, const Nerve* nerve, s32 maxStates);

    void initNerveAction(NerveActionCtrl* actionCtrl);
    void setNerve(const Nerve* nextNerve);
    void tryChangeNerve();
    void update();
    const Nerve* getCurrentNerve() const;

    s32 getCurrentStep() const { return mStep; }
    bool isNewNerve() const { return mNextNerve != nullptr; }
    NerveStateCtrl* getStateCtrl() const { return mStateCtrl; }
    NerveActionCtrl* getActionCtrl() const { return mActionCtrl; }

    template <typename T>
    T* getParent() {
        return static_cast<T*>(mParent);
    }

private:
    IUseNerve* mParent = nullptr;
    const Nerve* mCurrentNerve = nullptr;
    const Nerve* mNextNerve = nullptr;
    s32 mStep = 0;
    NerveStateCtrl* mStateCtrl = nullptr;
    NerveActionCtrl* mActionCtrl = nullptr;
};
}  // namespace al
