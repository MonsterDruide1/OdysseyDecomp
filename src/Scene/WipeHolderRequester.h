#pragma once

#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/IUseSceneObjHolder.h"

class WipeHolderRequester : public al::ISceneObj {
public:
    WipeHolderRequester();

    const char* getSceneObjName() const override;

    void disableOpenWipeForSkipDemo();
    bool isDisableOpenWipeForSkipDemo() const;
    void requestWipeClose(const char* wipeName);
    const char* getRequestWipeCloseInfoName() const;
    void requestWipeOpenImmidiate();
    bool tryReceiveRequestWipeOpenImmidiate();

private:
    bool mIsDisableOpenWipeForSkipDemo = false;
    const char* mRequestWipeCloseInfoName = nullptr;
    bool mIsRequestWipeOpenImmidiate = false;
};

static_assert(sizeof(WipeHolderRequester) == 0x20);

namespace rs {
void disableOpenWipeForSkipDemo(const al::IUseSceneObjHolder* user);
bool isDisableOpenWipeForSkipDemo(const al::IUseSceneObjHolder* user);
void requestWipeClose(const al::IUseSceneObjHolder* user, const char* wipeName);
const char* getRequestWipeCloseInfoName(const al::IUseSceneObjHolder* user);
void requestWipeOpenImmidiate(const al::IUseSceneObjHolder* user);
bool tryReceiveRequestWipeOpenImmidiate(const al::IUseSceneObjHolder* user);
}  // namespace rs
