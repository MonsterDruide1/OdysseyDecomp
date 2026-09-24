#pragma once

namespace al {
struct GameSystemInfo;

struct SequenceInitInfo {
    SequenceInitInfo(const GameSystemInfo* gameSystemInfo);

    const GameSystemInfo* gameSystemInfo;
};
}  // namespace al
