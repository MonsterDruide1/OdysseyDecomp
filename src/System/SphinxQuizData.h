#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class SphinxQuizData : public ByamlSave {
public:
    SphinxQuizData(s32 arraySize);
    void init();
    void answerCorrectSphinxQuiz(s32 index);
    void answerCorrectSphinxQuizAll(s32 index);
    bool isAnswerCorrectSphinxQuiz(s32 index) const;
    bool isAnswerCorrectSphinxQuizAll(s32 index) const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& reader) override;

private:
    bool* mIsAnswerCorrectSphinxQuiz;
    bool* mIsAnswerCorrectSphinxQuizAll;
    s32 mArraySize;
};
