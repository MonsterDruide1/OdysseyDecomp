#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class WipeSimple : public LayoutActor {
public:
    WipeSimple(const char* name, const char* layoutName, const LayoutInitInfo& info,
               const char* actorName);

    void startClose(s32 frames);
    void tryStartClose(s32 frames);
    void startCloseEnd();
    void startOpen(s32 frames);
    void tryStartOpen(s32 frames);
    bool isCloseEnd() const;
    bool isOpenEnd() const;
    void exeClose();
    void exeCloseEnd();
    void exeOpen();

    virtual void appear();

private:
    s32 mFrames = -1;
};

}  // namespace al
