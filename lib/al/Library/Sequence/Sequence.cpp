#include "Library/Sequence/Sequence.h"

#include "Library/Audio/AudioDirector.h"

namespace al {

Sequence::Sequence(const char* name) : NerveExecutor(name), mName(name) {}

Sequence::~Sequence() {
    if (mAudioDirector)
        mAudioDirector->finalize();
}

void Sequence::kill() {
    mIsAlive = false;
}

}  // namespace al
