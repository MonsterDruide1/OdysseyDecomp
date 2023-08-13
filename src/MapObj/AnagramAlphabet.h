#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"

class AnagramAlphabetCharacter;

class AnagramAlphabet : public al::LiveActor {
public:
    AnagramAlphabet(const char*);
    void init(const al::ActorInitInfo&);
    bool testBase(AnagramAlphabetCharacter*);
    bool testEndHack();
    bool testComplete();

    void exeWait();
    void exeComplete();
    void appearShine();
};
