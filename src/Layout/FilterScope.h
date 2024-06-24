#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class FilterScope : public al::LayoutActor {
public:
    FilterScope(const char* name, const al::LayoutInitInfo& info, const char* suffix);
    void startAppear();
    void end();
    void exeAppear();
    void exeWait();
    void exeEnd();
    bool isEnd() const;
};
