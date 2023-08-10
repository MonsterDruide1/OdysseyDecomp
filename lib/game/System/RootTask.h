#pragma once

#include <gfx/seadDrawContext.h>

class GameSystem;

// TODO: Implement sead::Task, currently missing from sead headers
class RootTask {
public:
    RootTask();
    void enter();
    void draw();
    void calc();
    void prepare();
    
private:
    // TODO: Remove once RootTask implements sead::Task
    char taskImplDetails[0x200];
    GameSystem* mGameSystem;
    void* field_208;
    sead::DrawContext* drawContext;
};
