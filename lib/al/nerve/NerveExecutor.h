#pragma once

#include "al/hio/HioNode.h"
#include "al/nerve/Nerve.h"
#include "al/nerve/NerveKeeper.h"

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