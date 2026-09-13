#pragma once

#include <prim/seadSafeString.h>

namespace al {
class HtmlViewer {
public:
    HtmlViewer();

    void call(const char*, sead::BufferedSafeStringBase<char>*) const;
};
}  // namespace al
