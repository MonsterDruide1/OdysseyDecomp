#pragma once

namespace al {
class Sequence;
}  // namespace al

namespace SequenceFactory {
al::Sequence* createSequence(const char* name);
}  // namespace SequenceFactory
