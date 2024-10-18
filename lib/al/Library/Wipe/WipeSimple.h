#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class WipeSimple : public LayoutActor {
public:
    WipeSimple(const char*, const char*, const LayoutInitInfo&, const char*);

    void startClose(s32);
    void tryStartClose(s32);
    void startCloseEnd();
    void startOpen(s32);
    void tryStartOpen(s32);
    bool isCloseEnd() const;
    bool isOpenEnd() const;
    void exeClose();
    void exeCloseEnd();
    void exeOpen();

    virtual void appear();

private:
    s32 mTime = -1;
};

}  // namespace al
