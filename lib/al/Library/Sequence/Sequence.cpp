#include "Library/Sequence/Sequence.h"

namespace al {

void Sequence::init(const SequenceInitInfo& initInfo) {}

void Sequence::kill() {
    mIsAlive = false;
}

bool Sequence::isDisposable() const {
    return true;
}

}  // namespace al
