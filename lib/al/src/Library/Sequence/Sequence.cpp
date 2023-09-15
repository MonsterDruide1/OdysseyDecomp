#include "Library/Sequence/Sequence.h"

namespace al {

void Sequence::init(const SequenceInitInfo& initInfo) {}
void Sequence::kill() {
    mIsAlive = false;
}

}  // namespace al
