#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
template <class T>
class DeriveActorGroup;

class FixMapParts;
class CameraTicket;
}  // namespace al

class AnagramAlphabetCharacter;
class Shine;
class SaveObjInfo;

class AnagramAlphabet : public al::LiveActor {
public:
    AnagramAlphabet(const char*);
    void init(const al::ActorInitInfo& info);
    bool testBase(AnagramAlphabetCharacter*);
    bool testEndHack();
    bool testComplete();

    void exeWait();
    void exeComplete();
    void appearShine();

private:
    al::DeriveActorGroup<AnagramAlphabetCharacter>* mAnagramAlphabetLetters;
    void* field_110;
    al::FixMapParts* mAnagramAlphabetBase;
    al::CameraTicket* mObjectCamera;
    Shine* mShineActor;
    SaveObjInfo* mSaveObjInfo;
};
