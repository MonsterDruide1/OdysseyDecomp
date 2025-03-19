#pragma once

#include <framework/seadTask.h>

class GameSystem;

namespace sead {
class DrawContext;
struct TaskConstructArg;

namespace hostio {
class Node;
}  // namespace hostio
}  // namespace sead

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
