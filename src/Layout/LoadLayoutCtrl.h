#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LayoutInitInfo;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class LoadLayoutCtrl : public al::NerveExecutor {
public:
    LoadLayoutCtrl(const al::LayoutInitInfo& info);

    void startWaitLoad();
    void endLoad();
    void exeWaitCount();
    void exeWaitLoad();
    void hideTxtPane();
    void exeEndLoad();

private:
    al::SimpleLayoutAppearWaitEnd* mSaveMessageLayout = nullptr;
};
