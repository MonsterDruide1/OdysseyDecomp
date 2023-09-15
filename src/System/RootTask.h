#pragma once

#include <framework/seadTask.h>
#include <gfx/seadDrawContext.h>
#include <hostio/seadHostIONode.h>

class GameSystem;

class RootTask : public sead::Task {
public:
    RootTask(const sead::TaskConstructArg& constructArg);
    ~RootTask() override;
    void enter() override;
    void draw() override;
    void calc() override;
    void prepare() override;

private:
    GameSystem* mGameSystem = nullptr;
    sead::hostio::Node* mHostIoNode = nullptr;
    sead::DrawContext* mDrawContext = nullptr;
};
