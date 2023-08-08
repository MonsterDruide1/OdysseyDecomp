#pragma once

#include "al/Library/HostIO/HioNode.h"
#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Nerve/NerveKeeper.h"

namespace al {
class NerveExecutor : public IUseNerve, public al::HioNode {
public:
    NerveExecutor(const char*);

    virtual NerveKeeper* getNerveKeeper() const;
    virtual ~NerveExecutor();

    void initNerve(const al::Nerve*, int);
    void updateNerve();

    al::NerveKeeper* mKeeper;  // _8
};
};  // namespace al
