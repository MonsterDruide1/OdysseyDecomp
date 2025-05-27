#pragma once

#include <heap/seadDisposer.h>

namespace al {
class SystemKit;
class GameFrameworkNx;
class AccountHolder;
struct DrawSystemInfo;
}  // namespace al

class RootTask;

class Application {
    SEAD_SINGLETON_DISPOSER(Application);

public:
    Application();
    void init(s32 argc, char* argv[]);
    void run();
    RootTask* getRootTask() const;

    al::SystemKit* getSystemKit() const { return mSystemKit; }

    al::GameFrameworkNx* getGameFramework() const { return mGameFramework; }

    al::DrawSystemInfo* getDrawSystemInfo() const { return mDrawSystemInfo; }

    al::AccountHolder* getAccountHolder() const { return mAccountHolder; }

private:
    al::SystemKit* mSystemKit = nullptr;
    al::GameFrameworkNx* mGameFramework = nullptr;
    al::DrawSystemInfo* mDrawSystemInfo = nullptr;
    al::AccountHolder* mAccountHolder = nullptr;
};

namespace ApplicationFunction {

void initialize(s32 argc, char** argv);

}
