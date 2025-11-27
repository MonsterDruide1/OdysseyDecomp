#include "Project/SaveData/SaveDataSequenceFormat.h"

namespace al {

SaveDataSequenceFormat::SaveDataSequenceFormat() = default;

s32 SaveDataSequenceFormat::threadFunc(const char* filename) {
    return -1;
}

void SaveDataSequenceFormat::start(s32 a, s32 b) {
    _8 = a;
    _c = b;
}

}  // namespace al
