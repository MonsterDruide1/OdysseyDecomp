#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseMessageSystem;
}  // namespace al

class TalkMessageInfoMessageParam {
public:
    TalkMessageInfoMessageParam(const al::IUseMessageSystem* messageSystem, const char16* message,
                                const char* label);
    void setting(const al::IUseMessageSystem* messageSystem, const char16* message);
    s32 getCharNum(s32 page) const;

private:
    s32 mPageCount = 0;
    s32* mCharNums = nullptr;
};

static_assert(sizeof(TalkMessageInfoMessageParam) == 0x10);
