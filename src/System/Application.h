#pragma once

#include <heap/seadDisposer.h>

namespace al {
class SystemKit;
class GameFrameworkNx;
class AccountHolder;
class GameDrawInfo;
}  // namespace al

class RootTask;

class Application {
    SEAD_SINGLETON_DISPOSER(Application);

public:
    Application();
    void init(s32 argc, char** argv);
    void run();
    RootTask* getRootTask() const;

    al::SystemKit* getSystemKit() const { return mSystemKit; }

    al::GameFrameworkNx* getGameFramework() const { return mGameFramework; }

    al::GameDrawInfo* getGameDrawInfo() const { return mGameDrawInfo; }

    al::AccountHolder* getAccountHolder() const { return mAccountHolder; }

protected:
    friend class ApplicationFunction;

private:
    al::SystemKit* mSystemKit;
    al::GameFrameworkNx* mGameFramework;
    al::GameDrawInfo* mGameDrawInfo;
    al::AccountHolder* mAccountHolder;
};
