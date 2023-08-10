#pragma once

#include <heap/seadDisposer.h>

namespace al {
class SystemKit;
class GameFrameworkNx;
class AccountHolder;
}  // namespace al

class RootTask;
struct GameDrawInfo;

class Application {
    friend class ApplicationFunction;
    SEAD_SINGLETON_DISPOSER(Application);
public:
    Application();
    void init(s32 argc, char** argv);
    void run();
    RootTask* getRootTask() const;

private:
    al::SystemKit* mSystemKit;
    al::GameFrameworkNx* mGameFramework;
    al::AccountHolder* mAccountHolder;
    GameDrawInfo* mDrawInfo;
};

namespace agl {
class RenderBuffer;
class DrawContext;
}  // namespace agl

struct GameDrawInfo {
    agl::RenderBuffer* dockedRenderBuffer;
    agl::RenderBuffer* handheldRenderBuffer;
    bool isDocked;
    agl::DrawContext* drawContext;
};
